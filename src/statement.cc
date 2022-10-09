#include "statement.hh"
#include "ast.hh"

ExpressionStatement::ExpressionStatement(Expression* expr) {
    this->expr = expr;
}

void ExpressionStatement::debug(size_t depth) {
    this->expr->debug(depth);
}

void ExpressionStatement::fold(AST* ast) {
    this->expr = this->expr->fold(ast);
}

llvm::Value* ExpressionStatement::codegen(AST* ast) {
    return this->expr->codegen(ast);
}

StatementBlock::StatementBlock(std::vector<Statement*> statements) {
    this->statements = statements;
}

void StatementBlock::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "Block {" << std::endl;
    for (size_t i = 0; i < this->statements.size(); i++) {
        std::cout << std::string((depth + 1) * 4 - 2, ' ') << "Statement: " << i << std::endl;
        this->statements[i]->debug(depth + 1);
    }
    std::cout << std::string(depth * 4, ' ') << "}" << std::endl;
}

void StatementBlock::fold(AST* ast) {
    ast->push_scope();
    for (size_t i = 0; i < this->statements.size(); i++) {
        this->statements[i]->fold(ast);
    }
    ast->pop_scope();
}

llvm::Value* StatementBlock::codegen(AST* ast) {
    for (long unsigned int i = 0; i < this->statements.size(); i++) {
        this->statements[i]->codegen(ast);
    }
    return nullptr;
}

ReturnStatement::ReturnStatement(Expression* expr) {
    this->expr = expr;
}

void ReturnStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ')<< "RETURN" << std::endl;
    this->expr->debug(depth + 1);
}

void ReturnStatement::fold(AST* ast) {
    this->expr = this->expr->fold(ast);
}

llvm::Value* ReturnStatement::codegen(AST* ast) {
    ast->Builder->CreateRet(this->expr->codegen(ast));
    return nullptr;
}

AssignmentStatement::AssignmentStatement(Expression* identifier, Expression* value, bool mut, AtomType type) {
    this->identifier = identifier;
    this->value = value;
    this->mut = mut;
    this->type = type;
}

void AssignmentStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ');
    std::cout << "let " << ((this->mut)?"mutable ":"static ") << ((ExpressionAtomic*)this->identifier)->str << " : ";
    switch (this->type) {
        case t_bool: std::cout << "bool "; break;
        case t_number: std::cout << "int "; break;
        case t_float: std::cout << "float "; break;
        case t_string: std::cout << "string "; break;
        default: std::cerr << "Unknown type for assignment"; break;
    }
    std::cout << " = " << std::endl;
    this->value->debug(depth + 1);
}

void AssignmentStatement::fold(AST* ast) {
    this->value = this->value->fold(ast);
    if (((ExpressionAtomic*)this->value)->type != this->type)
        ast->push_err("Attempted to assign incorrect type to variable");
    ast->scope->push_value(((ExpressionAtomic*)this->identifier)->str, new ScopeValue(this->value, this->mut));
}

llvm::Value* AssignmentStatement::codegen(AST* ast) {
    return nullptr;
}

IfStatement::IfStatement(Expression* condition, Statement* block1, Statement* block2) {
    this->condition = condition;
    this->block1 = block1;
    this->block2 = block2;
}

void IfStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "If" << std::endl;
    this->condition->debug(depth + 1);
    std::cout << std::string(depth * 4, ' ') << "Then" << std::endl;
    this->block1->debug(depth + 1);
    if (this->block2 != nullptr) {
        std::cout << std::string(depth * 4, ' ') << "Else" << std::endl;
        this->block2->debug(depth + 1);
    }
}

void IfStatement::fold(AST* ast) {
    this->condition = this->condition->fold(ast);
    this->block1->fold(ast);
    this->block2->fold(ast);
}

llvm::Value* IfStatement::codegen(AST* ast) {
    return nullptr;
}

WhileStatement::WhileStatement(Expression* cond, Statement* block) {
    this->condition = cond;
    this->block = block;
}

void WhileStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "While" << std::endl;
    this->condition->debug(depth + 1);
    std::cout << std::string(depth * 4, ' ') << "Do" << std::endl;
    this->block->debug(depth + 1);
}

void WhileStatement::fold(AST* ast) {
    this->condition = this->condition->fold(ast);
    this->block->fold(ast);
}

llvm::Value* WhileStatement::codegen(AST* ast) {
    return nullptr;
}
