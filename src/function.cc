#include "function.hh"

Function::Function(std::string name, Statement* statements, Type type, std::vector<std::vector<std::string>> args) {
    this->name = name;
    this->statements = statements;
    this->type = type;
    this->arg_count = args.size();
    this->indentifiers = std::vector<Expression*>();
    this->indentifier_type = std::vector<Type>();
    for (size_t i = 0; i < this->arg_count; i++) {
        this->indentifiers.push_back(new ExpressionAtomic(std::string(args[i][0]), true));
        if (args[i][0].compare("int")) {
            this->indentifier_type.push_back(t_number);
        } else if (args[i][0].compare("float")) {
            this->indentifier_type.push_back(t_float);
        } else if (args[i][0].compare("bool")) {
            this->indentifier_type.push_back(t_bool);
        } else if (args[i][0].compare("string")) {
            this->indentifier_type.push_back(t_string);
        }
    }
}

void Function::debug() {
    switch (this->type) {
        case t_number: std::cout << "int"; break;
        case t_float: std::cout << "float"; break;
        case t_string: std::cout << "str"; break;
        case t_null: std::cout << "void"; break;
        case t_bool: std::cout << "bool"; break;
        default: std::cerr << "Unknown function type"; break;
    }
    std::cout << " function " << this->name <<  std::endl;
    std::cout << this->arg_count << " argument" << ((this->arg_count != 1)? std::string("s") : std::string("")) << std::endl;
    for (size_t i = 0; i < this->arg_count; i++) {
        if (i != 0)
            std::cout << ", ";
        std::cout << ((ExpressionAtomic*)this->indentifiers[i])->str << " :";
        switch (this->indentifier_type[i]) {
            case t_number: std::cout << " int"; break;
            case t_float: std::cout << " float"; break;
            case t_bool: std::cout << " bool"; break;
            case t_string: std::cout << " string"; break;
            default: break;
        }
    }
    std::cout << std::endl;
    this->statements->debug(0);
}

void Function::fold(AST* ast) {
    this->statements->fold(ast);
}
