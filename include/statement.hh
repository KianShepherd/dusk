#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "expression.hh"

class Statement {
public:
    virtual void debug(size_t depth) {};
};


class ExpressionStatement : public Statement {
public:
    ExpressionStatement(Expression* exp);

    void debug(size_t depth) override;

private:
    Expression* expr;
};

class StatementBlock : public Statement {
public:
    StatementBlock(std::vector<Statement*> statements);

    void debug(size_t depth) override;

private:
    std::vector<Statement*> statements;
};

class ReturnStatement : public Statement {
public:
    ReturnStatement(Expression* exp);

    void debug(size_t depth) override;

private:
    Expression* expr;
};
