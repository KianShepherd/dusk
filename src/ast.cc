#include "ast.hh"

AST::AST() {
    this->statements = std::vector<Statement*>();
    this->error = false;
    this->err = std::stringstream();
}

void AST::push_statement(Statement* statement) {
    this->statements.push_back(statement);
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
    for (int i = 0; i < (int)this->statements.size(); i++) {
        this->statements[i]->debug(0);
    }
    std::cout << "-----DEBUG-----" << std::endl;
}
