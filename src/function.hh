#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "expression.hh"
#include "statement.hh"

class Function {
public:
    Function(std::string name, Statement* statements);
    void debug();
private:
    std::string name;
    Statement* statements;
};
