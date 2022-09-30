#pragma once
#include <string>
#include <iostream>
#include <vector>

enum Type {
    t_bool,
    t_number,
    t_float,
    t_identifier,
    t_string,
    t_null,
    t_function_call
};

class Expression {
public:
    virtual void debug(size_t depth) {};
};

class ExpressionAtomic : public Expression {
public:
    ExpressionAtomic(long long num);
    ExpressionAtomic(double num);
    ExpressionAtomic(std::string str, bool is_identifier);
    ExpressionAtomic(bool b);
    ExpressionAtomic(std::string str, std::vector<Expression*> args);
    ExpressionAtomic();

    void debug(size_t depth) override;
    std::string str;
private:
    double floating;
    long long number;
    bool boolean;
    Type type;
    std::vector<Expression*> args;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression* l, Expression* r, std::string t);

    void debug(size_t depth) override;
private:
    Expression* lhs;
    Expression* rhs;
    std::string type;
};

class UnaryExpression : public Expression {
public:
    UnaryExpression(Expression* expr, std::string op);

    void debug(size_t depth) override;
private:
    Expression* operand;
    std::string op;
};

class AssignmentExpression : public Expression {
public:
    AssignmentExpression(Expression* expr, Expression* op);

    void debug(size_t depth) override;
private:
    class Expression* identifier;
    class Expression* value;
};

class BreakExpression : public Expression {
public:
    BreakExpression();

    void debug(size_t depth) override;
};
