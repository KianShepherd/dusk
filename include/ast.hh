#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "expression.hh"
#include "statement.hh"
#include "function.hh"

class AST;

class ScopeFrame {
public:
    ScopeFrame();
    ScopeFrame(ScopeFrame* prev);
    ScopeFrame* new_scope();

    Expression* get_value(std::string identifier);
    void push_value(std::string identifier, Expression* expression);
    void update_value(AST* ast, std::string identifier, Expression* expression);

    ScopeFrame* prev_frame;
    ScopeFrame* next_frame;
private:
    std::map<std::string, Expression*> variables;
};

class AST {
public:
    AST();

    void push_function(Function* function);
    void push_err(std::string msg);

    void push_scope();
    void pop_scope();

    bool check_error(std::string stage);
    void debug();
    void fold_const_expressions();

    ScopeFrame* scope;
private:
    std::vector<Function*> functions;
    bool error;
    std::stringstream err;
};
