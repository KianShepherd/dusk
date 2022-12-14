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
#include <map>

#include "expression.hh"
#include "function.hh"

class AST;

class Struct {
public:
    Struct(std::string name, AST* ast);
    Struct(std::string name, AST* ast, bool is_template);
    void push_var(std::string name, AtomType);
    void push_var(std::string name, AtomType, std::string struct_name);
    void push_function(Function* func);
    void push_constructor(Function* func);
    void push_function_pre(Function* func);
    void finalize();
    Struct* monomorph(std::string new_name, std::string new_type);

    void debug(size_t depth);

    llvm::Value* codegen_functions(AST* ast);
    llvm::Value* codegen(AST* ast);

    std::string name;
    llvm::Type* struct_type;
    std::vector<Function*> member_functions;
    std::vector<Function*> constructors;
    std::vector<std::string> type_idents;
    std::vector<AtomType> types;
    std::map<std::string, int> gen_field_map; // Field map for llvm to use
    std::map<std::string, std::string> struct_var_map; // map of field name to struct name
    std::map<std::string, AtomType> struct_var_type_map; // map of field name to struct name
    std::map<std::string, llvm::Type*> field_type_map; // map of field name to llvm type

    AST* ast;
private:

    int mem_size;
    bool is_template;

    std::vector<std::string> func_idents;
    std::vector<llvm::Type*> llvm_types;
    std::vector<llvm::Function*> funcs;

    std::map<std::string, std::tuple<int, int>> field_map; // field type to access, field index to acess for member
};
