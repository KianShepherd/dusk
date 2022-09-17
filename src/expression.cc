#include "expression.hh"

ExpressionAtomic::ExpressionAtomic(long long num) {
    this->number = num;
    this->type = Type::t_number;
}

ExpressionAtomic::ExpressionAtomic(double num) {
    this->floating = num;
    this->type = Type::t_float;
}

ExpressionAtomic::ExpressionAtomic(std::string str, bool is_identifier) {
    this->str = std::string(str.substr(1, str.length() - 2));
    this->type = (is_identifier)?Type::t_identifier:Type::t_string;
}

ExpressionAtomic::ExpressionAtomic(bool b) {
    this->boolean = b;
    this->type = Type::t_bool;
}

ExpressionAtomic::ExpressionAtomic() {
    this->type = Type::t_null;
}

void ExpressionAtomic::debug() {
    switch (this->type) {
        case Type::t_number: std::cout << this->number << " : int"<< std::endl; break;
        case Type::t_float: std::cout << this->floating << " : float" << std::endl; break;
        case Type::t_string: std::cout << this->str << " - len = " << this->str.length() << " : str" << std::endl; break;
        case Type::t_identifier: std::cout << this->str << " - len = " << this->str.length() << " : identifier" << std::endl; break;
        case Type::t_null: std::cout << "null" << std::endl; break;
        case Type::t_bool: std::cout << ((this->boolean)?"True":"False") << std::endl; break;
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

UnaryExpression::UnaryExpression(Expression* expr, char* op) {
    this->operand = expr;
    this->op = 0;
}

void UnaryExpression::debug() {
    std::cout << "operand: ";
    this->operand->debug();
    
    std::cout << "op: " << this->op << std::endl;
}
