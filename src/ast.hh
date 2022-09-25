#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "expression.hh"
#include "statement.hh"
#include "function.hh"

class AST {
public:
    AST();

    void push_function(Function* function);
    void push_err(std::string msg);

    bool check_error();
    void debug();
private:
    std::vector<Function*> functions;
    bool error;
    std::stringstream err;
};
