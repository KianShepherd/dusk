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
#include <iterator>
#include "expression.hh"
#include "statement.hh"
#include "helpers.hh"

class AST;

class Function {
public:
    Function(std::string name, Statement* statements, AtomType type, std::vector<std::vector<std::string>> args);
    void debug();
    void fold(AST* ast);
    std::tuple<std::string, AtomType, std::vector<AtomType>> get_meta();

    llvm::Function* codegen(AST* ast);
    llvm::Function* codegen_proto(AST* ast);

    std::string name;
private:
    Statement* statements;
    AtomType type;
    size_t arg_count;
    std::vector<Expression*> indentifiers;
    std::vector<AtomType> indentifier_type;
    std::vector<bool> indentifiers_mutability;
};
