#include "my_std_lib_facilities.h"
#include "token.h"
#include "variable.h"
#include<cmath>


double factorial(double n) {
  double product{1.0};
  if (int(n) != n) error("factorial used with non-integer argument!");
  for (int i = 2; i <= n; i++)
    product *= i;
  return product;
}

double expression(Token_stream& ts);
double term(Token_stream& ts);
double primary(Token_stream& ts);

//Assume we have seen "let"
double declaration(Token_stream& ts) {
  Token t = ts.get();
  if (t.kind != name) error("name expected in declaration");
  std::string var_name = t.name;

  Token t2 = ts.get();
  if (t2.kind != '=') error("= missing in declaration of ", var_name);

  double d = expression(ts);
  define_name(var_name,d);
  return d;
}

double statement(Token_stream& ts) {
  Token t = ts.get();
  switch (t.kind) {
    case let:
      return declaration(ts);
    case name:
    {
      Token next = ts.get();
      if (next.kind == '=') {
        double d = expression(ts);
        set_value(t.name,d);
        return d;
      }
      ts.putback(next);
      ts.putback(t);
      return expression(ts);
    }
    default:
      ts.putback(t);
      return expression(ts);
  }
}

double expression(Token_stream& ts) {
  double left = term(ts);
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
      case '+':
        left += term(ts);
        t = ts.get();
        break;
      case '-':
        left -= term(ts);
        t = ts.get();
        break;
      case print: case ')':
        ts.putback(t);
        return left;
      default:
        if (print != '\n')
          error(std::string("Expected an operation (+,-,")  + print + ")");
        else
          error("Expected an operation (+,-)");
        ts.putback(t);
    }
  }
}

double term(Token_stream& ts) {
  double left = primary(ts);
  Token t = ts.get();
  while (true) {
    switch (t.kind) {
      case '(':
        ts.putback(t); //Leads into case '*' -- No break necessary.
      case '*':
        left *= primary(ts);
        t = ts.get();
        break;
      case '/':
      {
        double d = primary(ts);
        if (d == 0) error("Divide by zero");
        left /= d;
        t = ts.get();
        break;
      }
      case '%':
      {
        double d = primary(ts);
        if (d == 0) error("%:divide by zero");
        left = fmod(left,d);
        t = ts.get();
        break;
      }
      default:
        ts.putback(t);
        return left;
    }
  }
}

double primary(Token_stream& ts) {
  Token t = ts.get();
  switch (t.kind) {
    case '(':
    {
      double d = expression(ts);
      t = ts.get();
      if (t.kind != ')') error(") expected");
      Token next = ts.get();
      if (next.kind == '^')
        return pow(d,primary(ts));
      if (next.kind == '!') {
        while (next.kind == '!') {
          d = factorial(d);
          next = ts.get();
        }
        ts.putback(next);
        return d;
      }
      ts.putback(next);
      return d;
    }
    case number:
    {
      Token next = ts.get();
      if (next.kind == '^')
        return pow(t.value,primary(ts));
      if (next.kind == '!') {
          double d{t.value};
          while (next.kind == '!') {
            d = factorial(d);
            next = ts.get();
          }
          ts.putback(next);
          return d;
      }
      ts.putback(next);
      return t.value;
    }
    case name:
    {
      Token next = ts.get();
      if (next.kind == '^')
        return pow(get_value(t.name),primary(ts));
      if (next.kind == '!') {
        double d{get_value(t.name)};
        while (next.kind == '!') {
          d = factorial(d);
          next = ts.get();
        }
        ts.putback(next);
        return d;
      }
      ts.putback(next);
      return get_value(t.name);
    }
    case '-':
      return -primary(ts);
    case '+':
      return primary(ts);
    default:
      ts.putback(t);
      error("Primary expected");
  }
}