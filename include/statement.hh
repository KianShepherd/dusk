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

class AssignmentStatement : public Statement {
public:
    AssignmentStatement(Expression* identifier, Expression* value, bool mut, Type type);

    void debug(size_t depth) override;
private:
    Expression* identifier;
    Expression* value;
    bool mut;
    Type type;
};

class IfStatement : public Statement {
public:
    IfStatement(Expression* condition, Statement* block1, Statement* block2);

    void debug(size_t depth) override;
private:
    Expression* condition;
    Statement* block1;
    Statement* block2;
};

class WhileStatement : public Statement {
public:
    WhileStatement(Expression* cond, Statement* block);

    void debug(size_t depth) override;
private:
    Expression* condition;
    Statement* block;
};
