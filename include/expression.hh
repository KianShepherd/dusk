#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <string>
#include <iostream>
#include <vector>

class AST;
class ScopeValue;
class Struct;

enum AtomType {
    t_bool,
    t_number,
    t_long,
    t_float,
    t_identifier,
    t_string,
    t_char,
    t_null,
    t_function_call,
    t_number_arr,
    t_float_arr,
    t_bool_arr,
    t_string_arr,
    t_struct,
    t_dot_exp,
    t_get_struct,
    t_lvalue
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
    op_or,
    op_modulo
};


class Expression {
public:
    virtual void debug(size_t depth) {};
    virtual Expression* fold(AST* ast) { return nullptr; };
    virtual ExpType get_type() { return t_undef; };
    virtual AtomType get_atomic_type(AST* ast) { return t_null; };
    virtual AtomType get_atomic_type_keep_identifier(AST* ast) { return t_null; };

    virtual llvm::Value* codegen(AST* ast, AtomType type) = 0;
    virtual std::string type_str(AST* ast) { return std::string(""); };
};

class ExpressionAtomic : public Expression {
public:
    ExpressionAtomic(long num);
    ExpressionAtomic(long long num);
    ExpressionAtomic(double num);
    ExpressionAtomic(std::string str, bool is_identifier);
    ExpressionAtomic(std::string str, Expression* index);
    ExpressionAtomic(bool b);
    ExpressionAtomic(char c);
    ExpressionAtomic(std::string str, std::vector<Expression*> args);
    ExpressionAtomic();
    ExpressionAtomic(AtomType type, int length, std::vector<long long> values);
    ExpressionAtomic(AtomType type, int length, std::vector<double> values);
    ExpressionAtomic(int length, std::vector<std::string> values);
    ExpressionAtomic(Expression* base, Expression* operand);
    ExpressionAtomic(Expression* base, Struct* s, int index, std::string field_name);

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_atomic; };
    AtomType get_atomic_type(AST* ast) override;
    AtomType get_atomic_type_keep_identifier(AST* ast) override;
    uintptr_t get_value();

    llvm::Value* codegen(AST* ast, AtomType type) override;
    std::string type_str(AST* ast) override;

    std::string str;
    double floating;
    char character;
    long long number;
    bool boolean;
    AtomType type;
    std::vector<Expression*> args;
    int length;
    std::vector<long long> int_vals;
    std::vector<double> float_vals;
    std::vector<std::string> string_vals;
    Expression* index;
    Expression* base;
    Expression* operand;
    char dot_type;
    Struct* struct_t;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression* l, Expression* r, Operators t);

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_binary; };
    AtomType get_atomic_type(AST* ast) override;
    AtomType get_atomic_type_keep_identifier(AST* ast) override;

    llvm::Value* codegen(AST* ast, AtomType type) override;
    std::string type_str(AST* ast) override;
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
    AtomType get_atomic_type(AST* ast) override;
    AtomType get_atomic_type_keep_identifier(AST* ast) override;

    llvm::Value* codegen(AST* ast, AtomType type) override;
    std::string type_str(AST* ast) override;
private:
    Expression* operand;
    std::string op;
};

class AssignmentExpression : public Expression {
public:
    AssignmentExpression(Expression* expr, Expression* op);
    AssignmentExpression(Expression* expr, Expression* op, Expression* index);
    AssignmentExpression(Expression* expr, Expression* op, int is_methods);

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_assignment; };
    AtomType get_atomic_type(AST* ast) override;
    AtomType get_atomic_type_keep_identifier(AST* ast) override;

    llvm::Value* codegen(AST* ast, AtomType type) override;
    std::string type_str(AST* ast) override;
private:
    class Expression* identifier;
    class Expression* value;
    int is_methods;
    Expression* index;
    bool is_arr;
};

class BreakExpression : public Expression {
public:
    BreakExpression();

    void debug(size_t depth) override;
    Expression* fold(AST* ast) override;
    ExpType get_type() override { return t_break; };
    AtomType get_atomic_type(AST* ast) override;
    AtomType get_atomic_type_keep_identifier(AST* ast) override;

    llvm::Value* codegen(AST* ast, AtomType type) override;
    std::string type_str(AST* ast) override;
};
