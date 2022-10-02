#include "ast.hh"

AST::AST() {
    this->functions = std::vector<Function*>();
    this->error = false;
    this->err = std::stringstream();
    this->scope = new ScopeFrame();
}

void AST::push_function(Function* function) {
    this->functions.push_back(function);
}

void AST::push_err(std::string msg) {
    this->error = true;
    this->err << msg;
}

bool AST::check_error(std::string stage) {
    if (this->error) {
        std::cout << stage << this->err.str() << std::endl;
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

void AST::fold_const_expressions() {
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->functions[i]->fold(this);
    }
}

void AST::push_scope() {
    this->scope = this->scope->new_scope();
}

void AST::pop_scope() {
    if (this->scope->prev_frame == nullptr) {
        this->push_err("Popped too many scope frames.");
        this->check_error("Scope Error: ");
    } else {
        this->scope = this->scope->prev_frame;
        delete this->scope->next_frame;
    }
}

ScopeFrame::ScopeFrame() {
    this->prev_frame = nullptr;
    this->next_frame = nullptr;
    this->variables = std::map<std::string, Expression*>();
}

ScopeFrame::ScopeFrame(ScopeFrame* prev) {
    this->prev_frame = prev;
    this->next_frame = nullptr;
    this->variables = std::map<std::string, Expression*>();
}

ScopeFrame* ScopeFrame::new_scope() {
    ScopeFrame* next = new ScopeFrame(this);
    this->next_frame = next;
    return next;
}

void ScopeFrame::push_value(std::string identifier, Expression* expression) {
    this->variables[identifier] = expression;
}

Expression* ScopeFrame::get_value(std::string identifier) {
    auto iter = this->variables.find(identifier);
    if (iter != this->variables.end() ) {
        return iter->second;
    }
    if (this->prev_frame == nullptr)
        return nullptr;
    return this->prev_frame->get_value(identifier);
}
