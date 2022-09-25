#include "ast.hh"

AST::AST() {
    this->functions = std::vector<Function*>();
    this->error = false;
    this->err = std::stringstream();
}

void AST::push_function(Function* function) {
    this->functions.push_back(function);
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
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->functions[i]->debug();
    }
    std::cout << "-----DEBUG-----" << std::endl;
}
