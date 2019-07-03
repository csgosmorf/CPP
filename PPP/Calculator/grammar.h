#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "token.h"

double factorial(double n);

//Assume we have seen "let"
double declaration(Token_stream& ts);

double statement(Token_stream& ts);

double expression(Token_stream& ts);

double term(Token_stream& ts);

double primary(Token_stream& ts);

#endif