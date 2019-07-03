#include<iostream>
#include<string>
#include<cmath>
#include "my_std_lib_facilities.h"
#include "token.h"
#include "variable.h"
#include "grammar.h"


void clean_up_mess(Token_stream& ts) {
  ts.ignore(print);
}

void calculate() {
  Token_stream ts;
  while (std::cin)
  try {
    std::cout << prompt;
    Token t = ts.get();
    while (t.kind == print) t = ts.get();
    if (t.kind == quit) return;
    ts.putback(t);
    std::cout << result << statement(ts) << '\n';
  }
  catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    clean_up_mess(ts);
  }
}

int main() {
  try{
    define_name("pi", 3.1415926535);
    define_name("e", 2.7182818284);

    calculate();
    return 0;
  }
  catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    keep_window_open("~~");
    return 1;
  }
  catch (...) {
    std::cerr << "exception \n";
    keep_window_open("~~");
    return 2;
  }
}

/*
    Grammar:

    Expression:
      Expression + Term
      Expression - Term
    Term:
      Term * primary
      Term / Primary
      Term % primary
    Primary:
      FPL
      ( Expression )
      -primary
      +Primary
      Primary^Primary
      Primary!
*/
