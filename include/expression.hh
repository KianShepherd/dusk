#pragma once
#include <string>
#include <iostream>
#include <vector>

class AST;
class ScopeValue;

enum Type {
    t_bool,
    t_number,
    t_float,
    t_identifier,
    t_string,
    t_null,
    t_function_call,
};

enum ExpType {
    t_atomic,
    t_binary,
    t_unary,
    t_break,
    t_assignment,
    t_undef
};

enum Operators {
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_equal,
    op_not_equal,
    op_greater,
    op_less,
    op_greater_equal,
    op_less_equal,
    op_and,
    op_or
};

class Expression {
public:
    virtual void debug(size_t depth) {};
    virtual Expression* fold(AST* ast) { return nullptr; };
    virtual ExpType get_type() { return t_undef; };
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
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_atomic; };
    Type get_atomic_type() { return this->type; };
    uintptr_t get_value();
    std::string str;

    double floating;
    long long number;
    bool boolean;
    Type type;
    std::vector<Expression*> args;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression* l, Expression* r, Operators t);

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_binary; };
private:
    Expression* lhs;
    Expression* rhs;
    Operators type;
};

class UnaryExpression : public Expression {
public:
    UnaryExpression(Expression* expr, std::string op);

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_unary; };
private:
    Expression* operand;
    std::string op;
};

class AssignmentExpression : public Expression {
public:
    AssignmentExpression(Expression* expr, Expression* op);

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_assignment; };
private:
    class Expression* identifier;
    class Expression* value;
};

class BreakExpression : public Expression {
public:
    BreakExpression();

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_break; };
};
