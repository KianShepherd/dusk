#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "expression.hh"

class AST {
public:
    AST();

    void push_expr(Expression* expr);
    void push_err(std::string msg);

    bool check_error();
    void debug();
private:
    std::vector<Expression*> exprs;
    bool error;
    std::stringstream err;
};
