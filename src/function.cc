#include "function.hh"
#include "ast.hh"

Function::Function(std::string name, Statement* statements, AtomType type, std::vector<std::vector<std::string>> args) {
    this->name = name;
    this->struct_name = std::string("");
    this->statements = statements;
    this->type = type;
    this->arg_count = args.size();
    this->indentifiers = std::vector<Expression*>();
    this->indentifier_type = std::vector<AtomType>();
    this->indentifiers_mutability = std::vector<bool>();
    for (size_t i = 0; i < this->arg_count; i++) {
        this->indentifiers.push_back(new ExpressionAtomic(std::string(args[i][0]), true));
        if (args[i][1].compare("int") == 0) {
            this->indentifier_type.push_back(t_number);
        } else if (args[i][1].compare("long") == 0) {
            this->indentifier_type.push_back(t_long);
        } else if (args[i][1].compare("char") == 0) {
            this->indentifier_type.push_back(t_char);
        } else if (args[i][1].compare("float") == 0) {
            this->indentifier_type.push_back(t_float);
        } else if (args[i][1].compare("bool") == 0) {
            this->indentifier_type.push_back(t_bool);
        } else if (args[i][1].compare("intarr") == 0) {
            this->indentifier_type.push_back(t_number_arr);
        } else if (args[i][1].compare("floatarr") == 0) {
            this->indentifier_type.push_back(t_float_arr);
        } else if (args[i][1].compare("boolarr") == 0) {
            this->indentifier_type.push_back(t_bool_arr);
        } else if (args[i][1].compare("stringarr") == 0) {
            this->indentifier_type.push_back(t_string_arr);
        } else if (args[i][1].compare("string") == 0) {
            this->indentifier_type.push_back(t_string);
        } else if (args[i][1].compare("struct") == 0) {
            this->indentifier_type.push_back(t_struct);
            this->struct_names.push_back(args[i][3]);
        }
        if (args[i].size() == 3) {
            if (args[i][2].compare("t") == 0) {
                this->indentifiers_mutability.push_back(true);
            } else {
                this->indentifiers_mutability.push_back(false);
            }
        } else {
            this->indentifiers_mutability.push_back(false);
        }
    }
}

Function::Function(std::string name, Statement* statements, AtomType type, std::vector<std::vector<std::string>> args, std::string struct_name) {
    this->name = name;
    this->struct_name = struct_name;
    this->statements = statements;
    this->type = type;
    this->arg_count = args.size();
    this->indentifiers = std::vector<Expression*>();
    this->indentifier_type = std::vector<AtomType>();
    this->indentifiers_mutability = std::vector<bool>();
    for (size_t i = 0; i < this->arg_count; i++) {
        this->indentifiers.push_back(new ExpressionAtomic(std::string(args[i][0]), true));
        if (args[i][1].compare("int") == 0) {
            this->indentifier_type.push_back(t_number);
        } else if (args[i][1].compare("long") == 0) {
            this->indentifier_type.push_back(t_long);
        } else if (args[i][1].compare("char") == 0) {
            this->indentifier_type.push_back(t_char);
        } else if (args[i][1].compare("float") == 0) {
            this->indentifier_type.push_back(t_float);
        } else if (args[i][1].compare("bool") == 0) {
            this->indentifier_type.push_back(t_bool);
        } else if (args[i][1].compare("intarr") == 0) {
            this->indentifier_type.push_back(t_number_arr);
        } else if (args[i][1].compare("floatarr") == 0) {
            this->indentifier_type.push_back(t_float_arr);
        } else if (args[i][1].compare("boolarr") == 0) {
            this->indentifier_type.push_back(t_bool_arr);
        } else if (args[i][1].compare("stringarr") == 0) {
            this->indentifier_type.push_back(t_string_arr);
        } else if (args[i][1].compare("string") == 0) {
            this->indentifier_type.push_back(t_string);
        } else if (args[i][1].compare("struct") == 0) {
            this->indentifier_type.push_back(t_struct);
            this->struct_names.push_back(args[i][3]);
        }
        if (args[i][2].compare("t") == 0) {
            this->indentifiers_mutability.push_back(true);
        } else {
            this->indentifiers_mutability.push_back(false);
        }
    }
}

std::string Function::func_args_to_str() {
    auto str = std::string("");
    int struct_count = 0;
    for (long unsigned int i = 0; i < this->indentifier_type.size(); i++) {
        switch (this->indentifier_type[i]) {
            case t_number: str.append("int"); break;
            case t_long: str.append("long"); break;
            case t_char: str.append("char"); break;
            case t_float: str.append("float"); break;
            case t_bool: str.append("bool"); break;
            case t_string: str.append("string"); break;
            case t_struct: str.append(this->struct_names[struct_count++]); break;
            case t_number_arr: str.append("int*"); break;
            case t_float_arr: str.append("float*"); break;
            case t_bool_arr: str.append("bool*"); break;
            case t_string_arr: str.append("string*"); break;
            default: break;
        }
    }
    return str;
}

std::tuple<std::string, AtomType, std::vector<AtomType>> Function::get_meta() {
    return std::make_tuple(this->name, this->type, this->indentifier_type);
}

void Function::debug() {
    if (this->statements) {
        switch (this->type) {
            case t_number: std::cout << "int"; break;
            case t_long: std::cout << "long"; break;
            case t_char: std::cout << "char"; break;
            case t_float: std::cout << "float"; break;
            case t_string: std::cout << "str"; break;
            case t_struct: std::cout << "struct " << this->struct_name; break;
            case t_null: std::cout << "void"; break;
            case t_bool: std::cout << "bool"; break;
            case t_number_arr: std::cout << " int*"; break;
            case t_float_arr: std::cout << " float*"; break;
            case t_bool_arr: std::cout << " bool*"; break;
            case t_string_arr: std::cout << " string*"; break;
            default: std::cerr << "Unknown function type"; break;
        }
        std::cout << " function " << this->name <<  std::endl;
        std::cout << this->arg_count << " argument" << ((this->arg_count != 1)? std::string("s") : std::string("")) << std::endl;
        int struct_count = 0;
        for (size_t i = 0; i < this->arg_count; i++) {
            if (i != 0)
                std::cout << ", ";
            if (this->indentifiers_mutability[i]) {
                std::cout << "mut ";
            }
            std::cout << ((ExpressionAtomic*)this->indentifiers[i])->str << " :";
            switch (this->indentifier_type[i]) {
                case t_number: std::cout << " int"; break;
                case t_long: std::cout << " long"; break;
                case t_char: std::cout << " char"; break;
                case t_float: std::cout << " float"; break;
                case t_bool: std::cout << " bool"; break;
                case t_string: std::cout << " string"; break;
                case t_struct: std::cout << " struct " << this->struct_names[struct_count++]; break;
                case t_number_arr: std::cout << " int*"; break;
                case t_float_arr: std::cout << " float*"; break;
                case t_bool_arr: std::cout << " bool*"; break;
                case t_string_arr: std::cout << " string*"; break;
                default: break;
            }
        }
        std::cout << std::endl;
        this->statements->debug(0);
    }
}

void Function::fold(AST* ast) {
    ast->push_scope();
    int struct_count = 0;
    std::vector<Statement*> stats;
    ast->current_block = &stats;
    ast->block_returned = false;
    for (size_t i = 0; i < this->arg_count; i++) {
        ast->scope->push_value(((ExpressionAtomic*)this->indentifiers[i])->str, new ScopeValue(this->indentifiers_mutability[i], this->indentifier_type[i], ((this->indentifier_type[i] != t_struct)?std::string(""):this->struct_names[struct_count++])));

    }
    if (this->statements) {
        this->statements->fold(ast, stats);
        if (!((StatementBlock*)this->statements)->returned && this->name.find("__INCREF__") == std::string::npos && this->name.find("__DECREF__") == std::string::npos && this->name.find("__del__") == std::string::npos) {
            std::map<std::string, ScopeValue*> m = ast->scope->get_all();
            for(std::map<std::string, ScopeValue*>::iterator it = m.begin(); it != m.end(); ++it) {
                if (it->second->type == t_struct) {
                    ((StatementBlock*)this->statements)->statements.push_back(
                       new ExpressionStatement(
                           new ExpressionAtomic(
                               std::string(it->second->struct_name).append("__DECREF__").append(std::string(it->second->struct_name)),
                               std::vector<Expression*>({ new ExpressionAtomic(std::string(it->first), true) })
                           )
                       )
                   );
                }
        }
        }
    }
    ast->pop_scope();
    ast->current_block = nullptr;
    ast->block_returned = false;
}


Function* Function::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    std::string name = std::string(this->name);
    if (name.find(old_name) != std::string::npos) {
        name.replace(name.find(old_name), old_name.length(), new_name);
    }
    int strct_count = 0;
    auto args = std::vector<std::vector<std::string>>({});
    for (long unsigned int i = 0; i < this->arg_count; i++) {
        if (this->indentifier_type[i] != t_struct) {
            std::string type;
            switch (this->indentifier_type[i]) {
                case t_number: type =  "int"; break;
                case t_long: type =  "long"; break;
                case t_char: type =  "char"; break;
                case t_float: type =  "float"; break;
                case t_bool: type =  "bool"; break;
                case t_string: type =  "string"; break;
                case t_number_arr: type =  "int*"; break;
                case t_float_arr: type =  "float*"; break;
                case t_bool_arr: type =  "bool*"; break;
                case t_string_arr: type =  "string*"; break;
                default: type = std::string("unknown");
            }
            args.push_back(std::vector<std::string>({
                        ((ExpressionAtomic*)this->indentifiers[i])->str,
                        type,
                        ((this->indentifiers_mutability[i])?std::string("t"):std::string("f"))
                        }));
        } else {
            std::string s_name = std::string(this->struct_names[strct_count++]);
            if (s_name.compare(old_name) == 0) {
                s_name = new_name;
            } else if (s_name.compare(old_type) == 0) {
                s_name = new_type;
            }
            args.push_back(std::vector<std::string>({
                        std::string(((ExpressionAtomic*)this->indentifiers[i])->str),
                        std::string("struct"),
                        ((this->indentifiers_mutability[i])?std::string("t"):std::string("f")),
                        s_name
                        }));
        }
    }
    if (this->type != t_struct)
        return new Function(name, this->statements->monomorph(new_name, new_type, old_name, old_type), this->type, args);
    auto new_s_name = std::string(this->struct_name);
    if (new_s_name.find(old_name) != std::string::npos) {
        new_s_name = std::string(new_name);
    } else if (new_s_name.find(old_type) != std::string::npos) {
        new_s_name = std::string(new_type);
    }
    return new Function(name, this->statements->monomorph(new_name, new_type, old_name, old_type), this->type, args, new_s_name);
}

void Function::clean(AST* ast) {
    if (this->statements) {
        this->statements->clean(ast);
    }
}


llvm::Function* Function::codegen_proto(AST* ast) {
    llvm::Function *TheFunction = ast->TheModule->getFunction(this->name);

    if (!TheFunction) {
        // Make the function type:  double(double,double) etc.
        std::vector<llvm::Type*> func_args = std::vector<llvm::Type*>();
        
        for (size_t i = 0; i < this->arg_count; i++) {
            switch (this->indentifier_type[i]) {
                case t_number: func_args.push_back(llvm::Type::getInt64Ty(*(ast->TheContext))); break;
                case t_long: func_args.push_back(llvm::Type::getInt128Ty(*(ast->TheContext))); break;
                case t_char: func_args.push_back(llvm::Type::getInt8Ty(*(ast->TheContext))); break;
                case t_float: func_args.push_back(llvm::Type::getDoubleTy(*(ast->TheContext))); break;
                case t_bool: func_args.push_back(llvm::Type::getInt1Ty(*(ast->TheContext))); break;
                case t_string: func_args.push_back(llvm::Type::getInt8PtrTy(*(ast->TheContext))); break;
                case t_struct: func_args.push_back(llvm::Type::getInt8PtrTy(*(ast->TheContext))); break;
                case t_bool_arr: func_args.push_back(llvm::Type::getInt1PtrTy(*(ast->TheContext))); break;
                case t_number_arr: func_args.push_back(llvm::Type::getInt64PtrTy(*(ast->TheContext))); break;
                case t_float_arr: func_args.push_back(llvm::Type::getDoublePtrTy(*(ast->TheContext))); break;
                case t_string_arr: func_args.push_back(llvm::Type::getInt8PtrTy(*(ast->TheContext))->getPointerTo()); break;
                default: break;
            }
        }
        llvm::FunctionType *FT = nullptr;
        switch (this->type) {
            case t_number: FT = llvm::FunctionType::get(llvm::Type::getInt64Ty(*(ast->TheContext)), func_args, false); break;
            case t_long: FT = llvm::FunctionType::get(llvm::Type::getInt128Ty(*(ast->TheContext)), func_args, false); break;
            case t_char: FT = llvm::FunctionType::get(llvm::Type::getInt8Ty(*(ast->TheContext)), func_args, false); break;
            case t_float: FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*(ast->TheContext)), func_args, false); break;
            case t_string: FT = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(*(ast->TheContext)), func_args, false); break;
            case t_struct: FT = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(*(ast->TheContext)), func_args, false); break;
            case t_null: FT = llvm::FunctionType::get(llvm::Type::getVoidTy(*(ast->TheContext)), func_args, false); break;
            case t_bool: FT = llvm::FunctionType::get(llvm::Type::getInt1Ty(*(ast->TheContext)), func_args, false); break;
            case t_bool_arr: FT = llvm::FunctionType::get(llvm::Type::getInt1PtrTy(*(ast->TheContext)), func_args, false); break;
            case t_number_arr: FT = llvm::FunctionType::get(llvm::Type::getInt64PtrTy(*(ast->TheContext)), func_args, false); break;
            case t_float_arr: FT = llvm::FunctionType::get(llvm::Type::getDoublePtrTy(*(ast->TheContext)), func_args, false); break;
            case t_string_arr: FT = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(*(ast->TheContext))->getPointerTo(), func_args, false); break;
            default: FT = nullptr; break;
        }

        llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, this->name, ast->TheModule.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args()) {
            Arg.setName(((ExpressionAtomic*)this->indentifiers[Idx++])->str);
        }
        TheFunction = F;
    }
    return TheFunction;
}

llvm::Function* Function::codegen(AST* ast) {
    llvm::Function *TheFunction = ast->TheModule->getFunction(this->name);
    
    if (!TheFunction) {
        std::cout << "Function Codegen could not find: " << this->name << std::endl;
        return nullptr;
    }
    if (!this->statements)
        return TheFunction;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*(ast->TheContext), "entry", TheFunction);
    ast->Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    ast->NamedValues = ast->NamedValues->new_scope();
    auto Arg = TheFunction->args().begin();
    for (size_t i = 0; i < this->indentifier_type.size(); i++) {
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(ast, TheFunction, std::string(Arg->getName()), Arg->getType());
        ast->Builder->CreateStore(Arg, Alloca);

        // Add arguments to variable symbol table.
        ast->NamedValues->push_value(std::string(Arg->getName()), std::make_tuple(Alloca, Arg->getType(), this->indentifier_type[i], 1));
        Arg = std::next(Arg);
    }

    if (this->statements->codegen(ast))
        if (TheFunction->getReturnType()->isVoidTy())
            ast->Builder->CreateRetVoid();

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
}

void Function::push_front(Expression* name, AtomType type, bool mut, std::string struct_name) {
    this->indentifiers.insert(this->indentifiers.begin(), name);
    this->indentifier_type.insert(this->indentifier_type.begin(), type);
    this->indentifiers_mutability.insert(this->indentifiers_mutability.begin(), mut);
    this->arg_count++;
    this->struct_names.push_back(struct_name);
}
