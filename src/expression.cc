#include "expression.hh"

ExpressionAtomic::ExpressionAtomic(long long num) {
    this->number = num;
    this->type = t_number;
}

ExpressionAtomic::ExpressionAtomic(double num) {
    this->floating = num;
    this->type = t_float;
}

ExpressionAtomic::ExpressionAtomic(std::string str, bool is_identifier) {
    this->str = std::string(str.substr(1, str.length() - 2));
    this->type = (is_identifier)?t_identifier:t_string;
}

ExpressionAtomic::ExpressionAtomic(bool b) {
    this->boolean = b;
    this->type = t_bool;
}

ExpressionAtomic::ExpressionAtomic() {
    this->type = t_null;
}

void ExpressionAtomic::debug() {
    switch (this->type) {
        case t_number: std::cout << this->number << " : int"<< std::endl; break;
        case t_float: std::cout << this->floating << " : float" << std::endl; break;
        case t_string: std::cout << this->str << " - len = " << this->str.length() << " : str" << std::endl; break;
        case t_identifier: std::cout << this->str << " - len = " << this->str.length() << " : identifier" << std::endl; break;
        case t_null: std::cout << "null : null" << std::endl; break;
        case t_bool: std::cout << ((this->boolean)?"True : bool":"False : bool") << std::endl; break;
    }
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

UnaryExpression::UnaryExpression(Expression* expr, std::string op) {
    this->operand = expr;
    this->op = op;
}

void UnaryExpression::debug() {
    std::cout << "operand: ";
    this->operand->debug();
    
    std::cout << "op: " << this->op << std::endl;
}
