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

BinaryExpression::BinaryExpression(Expression l, Expression r, char* t) {
    this->lhs = l;
    this->rhs = r;
    this->type = 0;
}

UnaryExpression::UnaryExpression(Expression expr, char* op) {
    this->operand = expr;
    this->op = 0;
}
