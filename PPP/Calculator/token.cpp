#include<iostream>
#include "token.h"
#include "my_std_lib_facilities.h"

Token::Token() : kind{0} {}
Token::Token(char ch) : kind{ch} {}
Token::Token(char ch, double val) : kind{ch}, value{val} {}
Token::Token(char ch, std::string n) : kind{ch}, name{n} {}

// Read characters from cin and compose a Token
Token Token_stream::get() {
  //Check if we already have a Token ready
  if (capacity == 2) {
    capacity = 1;
    return buffer2;
  }
  if (capacity == 1) {
    capacity = 0;
    return buffer;
  }
  char ch;
  std::cin.get(ch);
  while (isspace(ch) && ch != '\n')
    std::cin.get(ch);

  switch (ch) {
    case quit:
    case print:
    case '(':
    case ')':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
    case '=':
    case '!':
      return Token{ch};
    case '.':
    {
      //May reach next case -- This is intentional.
      char next;
      std::cin >> next;
      std::cin.putback(next);
      if (!isdigit(next))
        error("Invalid token");
    }
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    {
      std::cin.putback(ch);
      double val;
      std::cin >> val;
      return Token{number,val};
    }
    default:
      if (isalpha(ch)) {
        std::string s;
        s += ch;
        while (std::cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
        std::cin.putback(ch);
        if (s == declkey) return Token{let};
        return Token{name,s};
      }
      error("Bad token");
  }
}

void Token_stream::putback(Token t) {
  if (capacity == 2) error("Tried to put back token when already full!");
  else if (capacity == 1) {
    buffer2 = t;
    capacity = 2;
  } else if (capacity == 0) {
    buffer = t;
    capacity = 1;
  } else {
    error("putback: Capacity is not 2,1, or 0");
  }
}

//c represents the kind of token
void Token_stream::ignore(char c) {

  if (capacity == 2 && c == buffer2.kind) {
    capacity = 1;
    return;
  }
  if (capacity == 1 && c == buffer.kind) {
    capacity = 0;
    return;
  }
  capacity = 0;

  //now search input
  while (std::cin.get() != c);
}
