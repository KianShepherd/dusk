#include "ast.hh"

AST::AST() {
    this->exprs = std::vector<Expression*>();
    this->error = false;
    this->err = std::stringstream();
}

void AST::push_expr(Expression* expr) {
    this->exprs.push_back(expr);
}

void AST::push_err(std::string msg) {
    this->error = true;
    this->err << msg;
}

bool AST::check_error() {
    if (this->error) {
        std::cout << this->err.str() << std::endl;
    }
    return this->error;
}

void AST::debug() {
    std::cout << "-----DEBUG-----" << std::endl;
    for (int i = 0; i < (int)this->exprs.size(); i++) {
        this->exprs[i]->debug(0);
    }
    std::cout << "-----DEBUG-----" << std::endl;
}
