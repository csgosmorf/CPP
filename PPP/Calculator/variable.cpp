#include <iostream>
#include "variable.h"
#include "my_std_lib_facilities.h"


//Table containing pi,e, and all user-defined variabless
std::vector<Variable> var_table;

//Returns the value of variable named s
double get_value(std::string s) {
  for (const Variable& v : var_table)
    if (v.name == s) return v.value;
  error("get: undefined variable ", s);
}

//set the Variable named s to d
void set_value(std::string s, double d) {
  for (Variable& v : var_table)
    if (v.name == s) {
      v.value = d;
      return;
    }
  error("set: undefined variable ", s);
}

//Return whether the name has already been declared
bool is_declared(std::string var) {
  for (const Variable& v : var_table)
    if (v.name == var) return true;
  return false;
}

//Declares and initializes a name
double define_name(std::string var, double val) {
  if (is_declared(var)) error(var," declared twice");
  var_table.push_back(Variable{var,val});
  return val;
}
