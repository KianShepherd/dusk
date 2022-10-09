#include "helpers.hh"
#include "ast.hh"

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
llvm::AllocaInst* CreateEntryBlockAlloca(AST* ast, llvm::Function *TheFunction, const std::string &VarName, llvm::Type* var_type) {
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*(ast->TheContext)), 0, VarName.c_str());
}
