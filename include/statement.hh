#pragma once
#include "llvm/ADT/APFloat.h"
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
#include "expression.hh"
#include "helpers.hh"

class AST;
class ScopeValue;

class Statement {
public:
    virtual void debug(size_t depth) {};
    virtual void fold(AST* ast, std::vector<Statement*>& block) {};
    virtual Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type);
    virtual void clean(AST* ast) {};
    virtual llvm::Value* codegen(AST* ast) = 0;
};


class ExpressionStatement : public Statement {
public:
    ExpressionStatement(Expression* exp);

    void debug(size_t depth) override;
    void fold(AST* ast, std::vector<Statement*>& block) override;
    Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) override;
    void clean(AST* ast) override;
    llvm::Value* codegen(AST* ast) override;
private:
    Expression* expr;
};

class StatementBlock : public Statement {
public:
    StatementBlock(std::vector<Statement*> statements);

    void debug(size_t depth) override;
    void fold(AST* ast, std::vector<Statement*>& block) override;
    Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) override;
    void clean(AST* ast) override;
    llvm::Value* codegen(AST* ast) override;

    std::vector<Statement*> statements;
    bool returned;
};

class ReturnStatement : public Statement {
public:
    ReturnStatement(Expression* exp);

    void debug(size_t depth) override;
    void fold(AST* ast, std::vector<Statement*>& block) override;
    Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) override;
    void clean(AST* ast) override;
    llvm::Value* codegen(AST* ast) override;
private:
    Expression* expr;
};

class AssignmentStatement : public Statement {
public:
    AssignmentStatement(Expression* identifier, Expression* value, bool mut, AtomType type, long length=0);
    AssignmentStatement(Expression* identifier, Expression* value, bool mut, AtomType type, std::string struct_name);

    void debug(size_t depth) override;
    void fold(AST* ast, std::vector<Statement*>& block) override;
    Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) override;
    void clean(AST* ast) override;
    llvm::Value* codegen(AST* ast) override;
private:
    Expression* identifier;
    Expression* value;
    bool mut;
    AtomType type;
    long length;
    std::string struct_name;
};

class IfStatement : public Statement {
public:
    IfStatement(Expression* condition, Statement* block1, Statement* block2);

    void debug(size_t depth) override;
    void fold(AST* ast, std::vector<Statement*>& block) override;
    Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) override;
    void clean(AST* ast) override;
    llvm::Value* codegen(AST* ast) override;
private:
    Expression* condition;
    Statement* block1;
    Statement* block2;
};

class WhileStatement : public Statement {
public:
    WhileStatement(Expression* cond, Statement* block);

    void debug(size_t depth) override;
    void fold(AST* ast, std::vector<Statement*>& block) override;
    Statement* monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) override;
    void clean(AST* ast) override;
    llvm::Value* codegen(AST* ast) override;
private:
    Expression* condition;
    Statement* block;
};
