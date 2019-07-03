#ifndef VARIABLE_H
#define VARIABLE_H

#include<vector>

//Used to store variables like pi, e, and variables user-defined at run-time
class Variable {
public:
  std::string name;
  double value;
};

//Return the value of the Variable named s
double get_value(std::string s);

//Set the Variable named s to d
void set_value(std::string s, double d);

//Return whether variable with name var has been declared
bool is_declared(std::string var);

//Declares and initializes a name
double define_name(std::string var, double val);

#endif