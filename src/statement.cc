#include "statement.hh"

ExpressionStatement::ExpressionStatement(Expression* expr) {
    this->expr = expr;
}

void ExpressionStatement::debug(size_t depth) {
    this->expr->debug(depth);
}

StatementBlock::StatementBlock(std::vector<Statement*> statements) {
    this->statements = statements;
}

void StatementBlock::debug(size_t depth) {
    for (size_t i = 0; i < this->statements.size(); i++) {
        std::cout << "Statement: " << i << std::endl;
        this->statements[i]->debug(depth);
    }
}
