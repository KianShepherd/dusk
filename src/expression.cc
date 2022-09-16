#include "expression.hh"

ExpressionAtomic::ExpressionAtomic(long long num) {
    this->number = num;
    this->type = 0;
}

ExpressionAtomic::ExpressionAtomic(double num) {
    this->floating = num;
    this->type = 1;
}

ExpressionAtomic::ExpressionAtomic(char* str) {
    this->str = std::string(str);
    this->type = 2;
}

void ExpressionAtomic::debug() {
    std::cout << this->number << " " << this->type << std::endl;
}

BinaryExpression::BinaryExpression(Expression* l, Expression* r, std::string t) {
    this->lhs = l;
    this->rhs = r;
    this->type = t;
}

void BinaryExpression::debug() {
    std::cout << "lhs: ";
    this->lhs->debug();

    std::cout << "rhs: ";
    this->rhs->debug();
    
    std::cout << "type: " << this->type << std::endl;
}

UnaryExpression::UnaryExpression(Expression* expr, char* op) {
    this->operand = expr;
    this->op = 0;
}

void UnaryExpression::debug() {
    std::cout << "operand: ";
    this->operand->debug();
    
    std::cout << "op: " << this->op << std::endl;
}
