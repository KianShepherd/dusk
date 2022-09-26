#pragma once
#include <string>
#include <iostream>

enum Type {
    t_bool,
    t_number,
    t_float,
    t_identifier,
    t_string,
    t_null
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
    ExpressionAtomic();

    void debug(size_t depth) override;
private:
    std::string str;
    double floating;
    long long number;
    bool boolean;
    Type type;
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