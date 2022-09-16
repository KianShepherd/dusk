#include <string>


class Expression {

};

class ExpressionAtomic : public Expression {
public:
    ExpressionAtomic(long long num);
    ExpressionAtomic(double num);
    ExpressionAtomic(char* str);
private:
    std::string str;
    double floating;
    long long number;
    char type;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression l, Expression r, char* t);
private:
    Expression lhs;
    Expression rhs;
    char type;
};

class UnaryExpression : public Expression {
public:
    UnaryExpression(Expression expr, char* op);
private:
    Expression operand;
    char op;
};
