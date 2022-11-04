#include "struct.hh"
#include "ast.hh"

llvm::Type* atom_to_type(AtomType type, AST* ast) {
    switch (type) {
        case t_bool: return llvm::Type::getInt1Ty(*(ast->TheContext));
        case t_char: return llvm::Type::getInt8Ty(*(ast->TheContext));
        case t_number: return llvm::Type::getInt64Ty(*(ast->TheContext));
        case t_long: return llvm::Type::getInt128Ty(*(ast->TheContext));
        case t_float: return llvm::Type::getDoubleTy(*(ast->TheContext));

        case t_string: return llvm::Type::getInt8PtrTy(*(ast->TheContext)); 
        case t_number_arr: return llvm::Type::getInt64PtrTy(*(ast->TheContext));
        case t_float_arr: return llvm::Type::getDoublePtrTy(*(ast->TheContext));
                    
        default: return llvm::Type::getInt8PtrTy(*(ast->TheContext)); 
    }
}

std::string atom_to_debug(AtomType type) {
    switch (type) {
        case t_bool: return std::string("bool");
        case t_char: return std::string("char");
        case t_number: return std::string("int");
        case t_long: return std::string("long");
        case t_float: return std::string("float");

        case t_string: return std::string("char*/void*");
        case t_number_arr: return std::string("int*");
        case t_float_arr: return std::string("float*");
                    
        default: return std::string("void*");
    }
}

int atom_to_size(AtomType type) {
    switch (type) {
        case t_bool: return sizeof(char);
        case t_char: return sizeof(char);
        case t_number: return sizeof(long);
        case t_long: return sizeof(long long);
        case t_float: return  sizeof(double);

        case t_string: return sizeof(char*);
        case t_number_arr: return sizeof(long*);
        case t_float_arr: return sizeof(double*);
                    
        default: return sizeof(void*);
    }
}

Struct::Struct(std::string name, AST* ast) {
    this->name = name;
    this->ast = ast;
}

void Struct::finalize() {
    auto base_const_name = std::string("~").append(this->name).append("~");
    this->member_functions.push_back(
        new Function(
            base_const_name,
            new StatementBlock(
                std::vector<Statement*>(
                    {
                        new ReturnStatement(
                            new ExpressionAtomic(
                                "newstruct",
                                std::vector<Expression*>({ new ExpressionAtomic((long)this->mem_size) })
                            )
                        )
                    }
                )
            ),
            t_string,
            std::vector<std::vector<std::string>>()
        )
    );
    for (auto& con : this->constructors) {
        // delete self arg, we add a statement to the top to instantiate self.
        con->indentifiers.erase(con->indentifiers.begin());
        con->indentifier_type.erase(con->indentifier_type.begin());
        con->indentifiers_mutability.erase(con->indentifiers_mutability.begin());
        con->struct_names.erase(con->struct_names.begin());
        con->arg_count--;

        // Handling for constructor overloading attach num_args and arg types to func_name
        con->name.append(std::to_string(((int)con->indentifiers.size())));
        con->name.append(con->func_args_to_str());

        // First line of function declares self
        ((StatementBlock*)con->statements)->statements.insert(((StatementBlock*)con->statements)->statements.begin(), new AssignmentStatement(new ExpressionAtomic("self", true), new ExpressionAtomic(base_const_name, std::vector<Expression*>()), true, t_struct, this->name));
        this->member_functions.push_back(con);
    }
    this->struct_type = llvm::StructType::create(*(this->ast->TheContext), this->llvm_types, this->name, true);
}

void Struct::debug(size_t depth) {
    std::cout << std::endl << "Struct "<< this->name << " {" << std::endl;;

    for (int i = 0; i < this->type_idents.size(); i++) {
        std::cout << std::string(4, ' ') << this->type_idents[i] << " : " << atom_to_debug(this->types[i]);
        if (this->struct_var_map.find(this->type_idents[i]) != this->struct_var_map.end()) {
            std::cout << " " << this->struct_var_map[this->type_idents[i]];
        }
        std::cout << " : " << this->gen_field_map[this->type_idents[i]] << "," << std::endl;
    }
    std::cout << "}" << std::endl;

}

void Struct::push_var(std::string name, AtomType type) {
    int idx = this->type_idents.size();
    this->type_idents.push_back(name);
    this->gen_field_map[name] = idx;
    int field_idx = this->types.size();
    this->types.push_back(type);
    auto llvm_type = atom_to_type(type, this->ast);
    this->llvm_types.push_back(llvm_type);
    this->field_map[name] = std::tuple<int, int>(0, field_idx);
    this->mem_size += atom_to_size(type);
    this->field_type_map[name] = llvm_type;
    this->struct_var_map[name] = type;

}

void Struct::push_var(std::string name, AtomType type, std::string struct_name) {
    int idx = this->type_idents.size();
    this->type_idents.push_back(name);
    this->gen_field_map[name] = idx;
    int field_idx = this->types.size();
    this->types.push_back(type);
    auto llvm_type = atom_to_type(type, this->ast);
    this->llvm_types.push_back(llvm_type);
    this->field_map[name] = std::tuple<int, int>(0, field_idx);
    this->mem_size += atom_to_size(type);
    if (type == t_struct) {
        this->struct_var_map[name] = struct_name;
    }
    this->field_type_map[name] = llvm_type;
    this->struct_var_map[name] = type;
}

void Struct::push_function(Function* func) {
    if (!func->statements) {
        this->member_functions.push_back(func);
        return;
    }
    bool bin_op = false;
    if (func->name.compare("__add__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__sub__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__mul__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__div__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__eq__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__neq__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__gt__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__lt__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__ge__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__le__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__and__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__or__") == 0) {
        bin_op = true;
    } else if (func->name.compare("__mod__") == 0) {
        bin_op = true;
    }

    if (!bin_op) {
        std::cout << "not binop" << std::endl;
        if (((ExpressionAtomic*)func->indentifiers[0])->str.compare(std::string("self")) != 0 || func->indentifier_type[0] != t_struct) {
            auto err_msg = std::string("All struct member functions must start with the argument self and matching type (");
            err_msg.append(std::string(this->name));
            err_msg.append(std::string(")\n"));
            std::cout << "ERROR: " << err_msg << std::endl;
            this->ast->push_err(err_msg);
        }
        if (func->name.compare(this->name) == 0) {
            this->constructors.push_back(func);
            return;
        }
        std::string func_name = std::string(this->name).append(func->name);
        this->func_idents.push_back(func_name);
        this->gen_field_map[func_name] = -1;
        int field_idx = this->member_functions.size();
        this->field_map[func_name] = std::tuple<int, int>(1, field_idx);
        func->name = func_name;
        this->member_functions.push_back(func);
    } else {
        std::cout << "binop" << std::endl;
        if (func->arg_count != 2)
            this->ast->push_err(std::string("Binary operators only allow 2 arguments."));
        std::string func_name = func->func_args_to_str();
        func_name.append(func->name);
        func->name = func_name;
        this->member_functions.push_back(func);
    }
}

llvm::Value* Struct::codegen_functions(AST* ast) {
    for (auto& f : this->member_functions) {
        if (f->statements)
            this->funcs.push_back(f->codegen(ast));
    }
    return nullptr;
}

llvm::Value* Struct::codegen(AST* ast) {
     return nullptr;
}
