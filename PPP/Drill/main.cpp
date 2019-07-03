#include "std_lib_facilities.h"

using namespace std;

bool isPrime(int n) {
    for (int i = 2; i*i <= n; i+=2) {
        if (n % i == 0) return false;
    }
    return true;
}

vector<int>& primes() {
    static vector<int> out_primes;
    if (out_primes.size() > 0) return out_primes;
    int max_integer = 1 << 20;
    out_primes.push_back(2);
    for (int i = 3; i < max_integer; i+=2) {
        bool isPrime = true;
        for (int& prime : out_primes) {
            if (i % prime == 0) {
                isPrime = false;
                break;
            }
            if (prime * prime > i)
                break;
        }
        if (isPrime)
            out_primes.push_back(i);
    }
    return out_primes;
}

class Rational {
    public:
        Rational();
        Rational(int n, int d);
        Rational(int n);
        int nom() const {return top;}
        int denom() const {return bot;}
        double to_double() const {return double(top) / bot;}
        void simplify();
    private:
        int top{0};
        int bot{1};
};

Rational::Rational() : top{0}, bot{1} {}
Rational::Rational(int n) : top{n}, bot{1} {}
Rational::Rational(int n, int d) : top{n}, bot{d} {}
void Rational::simplify() {
    int top_copy = top;
    int bot_copy = bot;
    vector<int> top_primes;
    for (int& prime : primes()) {
        if (prime * prime > top_copy && prime * prime > bot_copy)
            break;
        if (top_copy % prime == 0) {
            top_primes.push_back(prime);
            do {top_copy /= prime;}
            while (top_copy % prime == 0);
        }
    }
    for (int& top_prime : top_primes) {
        if (top_prime * top_prime > top && top_prime * top_prime > bot)
            break;
        while (top % top_prime == 0 && bot % top_prime == 0) {
            top /= top_prime;
            bot /= top_prime;
        }
    }
}

Rational operator+(const Rational& a, const Rational& b) {
    Rational sum{a.nom() * b.denom() + b.nom() * a.denom(),a.denom() * b.denom()};
    sum.simplify();
    return sum;
}

Rational operator-(const Rational& a, const Rational& b) {
    Rational difference{a.nom() * b.denom() - b.nom() * a.denom(), a.denom() * b.denom()};
    difference.simplify();
    return difference;
}

Rational operator*(const Rational& a, const Rational& b) {
    Rational product{a.nom() * b.nom(), a.denom() * b.denom()};
    product.simplify();
    return product;
}

Rational operator/(const Rational& a, const Rational& b) {
    Rational quotient{a.nom() * b.denom(), a.denom() * b.nom()};
    quotient.simplify();
    return quotient;
}

Rational operator-(const Rational& a) {
    Rational negated{-a.nom(), a.denom()};
    return negated;
}

Rational pow(const Rational& a, int n) {
    if (n < 0) return Rational{1,1} / pow(a, -n);
    if (n == 0) return Rational{1,1};
    if (n % 2 == 0) return pow(a*a, n/2);
    return Rational{n,1} * pow(a,n-1);
}

ostream& operator<<(ostream& os, Rational a) {
    if (a.denom() == 1)
        return os << a.nom();

    return os << a.nom() << '/' << a.denom();
}

istream& operator>>(istream& is, Rational& a) {
    int nom{0}, denom{1};
    char op;
    
    cin >> nom;
    cin.get(op);
    if (op == '/') {
        char ch;
        cin >> ch;
        cin.putback(ch);
        if (isdigit(ch))
            cin >> denom;
        else
            cin.putback(op);
    }
    else 
        cin.putback(op);
    a = Rational{nom,denom};
    a.simplify();
    return is;
}

const char rational = 'r';
const char print = '\n';
const string prompt = "> ";
const string result = "= ";

struct Token {
    char kind;
    Rational value;
};

class Token_stream {
public:
    Token get();
    void put_back(Token t);
    void ignore(char c);
private:
    bool full{false};
    Token buffer;
};

void Token_stream::ignore(char c) {
    if (full && buffer.kind == c) {
        full = false;
        return;
    }
    char ch = c+1; //Just so it is not c
    while (ch != c) {
        cin.get(ch);
    }
}

Token Token_stream::get() {
    if (full) {
        full = false;
        return buffer;
    }
    char ch;
    cin.get(ch);
    while (isspace(ch) && ch != '\n') {
        cin.get(ch);
    }
    switch (ch) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);
            Rational a;
            cin >> a;
            return Token{rational,a};
        }
        case '+': case '-': case '*': case '/':
        case '(': case ')':
            return Token{ch};
        case print:
            return Token{print};
        default:
            error("Bad Token!");
    }
}

void Token_stream::put_back(Token t) {
    if (full) {
        error("Tried to putback token when full!");
    } else {
        buffer = t;
        full = true;
    }
}

Rational expression(Token_stream& ts);

Rational primary(Token_stream& ts) {
    Token t = ts.get();
    switch (t.kind) {
        case rational:
            Token next = ts.get();
            if (next.kind == '^') {
                return pow(t.value, primary(ts));
            }
            return t.value;
        case '-':
            return -primary(ts);
        case '+':
            return primary(ts);
        case '(':
        {
            Rational val = expression(ts);
            if (ts.get().kind != ')') error(") expected in primary");
            return val;
        }
        default:
            ts.put_back(t);
            error("Primary expected");
    }
}

Rational term(Token_stream& ts) {
    Rational left = primary(ts);
    Token t = ts.get();
    while (true)
    switch (t.kind) {
        case '*':
            left = left * primary(ts);
            t = ts.get();
            break;
        case '/':
            left = left / primary(ts);
            t = ts.get();
            break;
        default:
            ts.put_back(t);
            return left;
    }
}

Rational expression(Token_stream& ts) {
    Rational left = term(ts);
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
            case '+':
                left = left + term(ts);
                t = ts.get();
                break;
            case '-':
                left = left - term(ts);
                t = ts.get();
                break;
            case ')': case print:
                ts.put_back(t);
                return left;
            default:
                cerr << t.kind << '\n';
                error("Expected operation (+,-) in expression");
        }
    }
}

void clean_up_mess(Token_stream& ts) {
    ts.ignore(print);
}

void calculate(Token_stream& ts) {
    while (true)
    try {
        cout << prompt;
        Rational ans = expression(ts);
        Token t = ts.get();
        if (t.kind == print) {
            if (ans.denom() == 1)
                cout << result << ans << '\n';
            else 
                cout << result << ans << " or " << ans.to_double() << '\n';
        }
    } catch (exception& e) {
        cerr << e.what() << '\n';
        clean_up_mess(ts);
    }
}

int main() {
    Token_stream ts;
    try {
        calculate(ts);
    } catch (exception& e) {
        cerr << e.what();
        keep_window_open("~~");
        return 1;
    } catch (...) {
        keep_window_open("~~");
        return 2;
    }
    return 0;
}