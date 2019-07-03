#ifndef TOKEN_H
#define TOKEN_H

#include<string>

//Symbolic constants used to represent kinds of tokens
constexpr char number{'8'};
constexpr char quit{'q'};
constexpr char print{'\n'};
constexpr char name{'a'}; //The name of a variable
constexpr char let{'L'};

//Constants for I/O interface
const std::string prompt{"> "};
const std::string result{"= "};
const std::string declkey{"let"};

class Token {
public:
  char kind;
  double value;
  std::string name;
  Token();
  Token(char ch);
  Token(char ch, double val);
  Token(char ch, std::string n);
};

class Token_stream {
public:
  Token get();
  void putback(Token t);
  void ignore(char c);
private:
  Token buffer;
  Token buffer2;
  int capacity{0}; // Number of buffers used
};

#endif
