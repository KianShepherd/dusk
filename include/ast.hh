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
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/LegacyPassNameParser.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/MC/SubtargetFeature.h"
#include "llvm/InitializePasses.h"
#include "llvm/LinkAllIR.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/RegionPass.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Bitcode/BitcodeWriterPass.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Config/llvm-config.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/IPO/AlwaysInliner.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/SystemUtils.h"
#include "llvm/Support/SourceMgr.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "expression.hh"
#include "statement.hh"
#include "function.hh"
#include "struct.hh"

class AST;

class ScopeValue {
public:
    ScopeValue(bool mut, AtomType type);
    bool mut;
    AtomType type;
};

class ScopeFrame {
public:
    ScopeFrame();
    ScopeFrame(ScopeFrame* prev);
    ScopeFrame* new_scope();

    AtomType get_value(std::string identifier);
    void push_value(std::string identifier, ScopeValue* value);
    void update_value(AST* ast, std::string identifier, ScopeValue* value);

    ScopeFrame* prev_frame;
    ScopeFrame* next_frame;
private:
    std::map<std::string, ScopeValue*> variables;
};

class CodegenScopeFrame {
public:
    CodegenScopeFrame();
    CodegenScopeFrame(CodegenScopeFrame* prev);
    CodegenScopeFrame* new_scope();

    std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long> get_value(std::string identifier);
    void push_value(std::string identifier, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long> value);
    void update_value(std::string identifier, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long>);

    CodegenScopeFrame* prev_frame;
    CodegenScopeFrame* next_frame;
private:
    std::map<std::string, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long>> NamedValues;
};

class AST {
public:
    AST();

    void push_function(Function* function);
    void push_struct(Struct* s);
    void push_err(std::string msg);

    void push_scope();
    void pop_scope();

    bool check_error(std::string stage);
    void debug();
    void static_checking();
    llvm::Value* LogErrorV(const char *Str);

    void codegen(char debug, bool optimizations, std::string outfile);
    void stdlib();

    ScopeFrame* scope;
    CodegenScopeFrame* NamedValues;
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;

    std::vector<std::tuple<std::string, AtomType, std::vector<AtomType>>> func_definitions;
private:
    std::vector<Function*> functions;
    std::vector<Struct*> structs;
    bool error;
    std::stringstream err;
};
