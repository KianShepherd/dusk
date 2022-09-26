#include "function.hh"

Function::Function(std::string name, Statement* statements, Type type) {
    this->name = name;
    this->statements = statements;
    this->type = type;
}

void Function::debug() {
    switch (this->type) {
        case t_number: std::cout << "int"; break;
        case t_float: std::cout << "float"; break;
        case t_string: std::cout << "str"; break;
        case t_null: std::cout << "void"; break;
        case t_bool: std::cout << "bool"; break;
        default: break;
    }
    std::cout << " function " << this->name <<  std::endl;
    this->statements->debug(0);
}
