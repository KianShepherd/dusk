#pragma once
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "expression.hh"
#include "statement.hh"
#include "function.hh"

class AST;

class ScopeValue {
public:
    ScopeValue(Expression* value, bool mut);
    Expression* value;
    bool mut;
};

class ScopeFrame {
public:
    ScopeFrame();
    ScopeFrame(ScopeFrame* prev);
    ScopeFrame* new_scope();

    Expression* get_value(std::string identifier);
    void push_value(std::string identifier, ScopeValue* value);
    void update_value(AST* ast, std::string identifier, ScopeValue* value);

    ScopeFrame* prev_frame;
    ScopeFrame* next_frame;
private:
    std::map<std::string, ScopeValue*> variables;
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
    llvm::Value* LogErrorV(const char *Str);

    void codegen();

    ScopeFrame* scope;
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::map<std::string, llvm::Value*> NamedValues;
private:
    std::vector<Function*> functions;
    bool error;
    std::stringstream err;
};
