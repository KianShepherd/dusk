#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "expression.hh"
#include "statement.hh"

class AST {
public:
    AST();

    void push_statement(Statement* statement);
    void push_err(std::string msg);

    bool check_error();
    void debug();
private:
    std::vector<Statement*> statements;
    bool error;
    std::stringstream err;
};
