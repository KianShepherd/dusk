#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "expression.hh"
#include "statement.hh"

class Function {
public:
    Function(std::string name, Statement* statements, Type type, std::vector<std::vector<std::string>> args);
    void debug();
private:
    std::string name;
    Statement* statements;
    Type type;
    size_t arg_count;
    std::vector<Expression*> indentifiers;
    std::vector<Type> indentifier_type;
};
