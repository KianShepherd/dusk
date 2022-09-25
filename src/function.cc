#include "function.hh"

Function::Function(std::string name, Statement* statements) {
    this->name = name;
    this->statements = statements;
}

void Function::debug() {
    std::cout << "Function name: " << this->name <<  std::endl;
    this->statements->debug(0);
}
