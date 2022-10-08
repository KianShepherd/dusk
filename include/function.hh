#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "expression.hh"
#include "statement.hh"

class AST;

class Function {
public:
    Function(std::string name, Statement* statements, AtomType type, std::vector<std::vector<std::string>> args);
    void debug();
    void fold(AST* ast);
private:
    std::string name;
    Statement* statements;
    AtomType type;
    size_t arg_count;
    std::vector<Expression*> indentifiers;
    std::vector<AtomType> indentifier_type;
};
