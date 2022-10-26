#include "function.hh"
#include "ast.hh"

Function::Function(std::string name, Statement* statements, AtomType type, std::vector<std::vector<std::string>> args) {
    this->name = name;
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

std::tuple<std::string, AtomType, std::vector<AtomType>> Function::get_meta() {
    return std::make_tuple(this->name, this->type, this->indentifier_type);
}
void Function::debug() {
    switch (this->type) {
        case t_number: std::cout << "int"; break;
        case t_long: std::cout << "long"; break;
        case t_char: std::cout << "char"; break;
        case t_float: std::cout << "float"; break;
        case t_string: std::cout << "str"; break;
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
            case t_number_arr: std::cout << " int*"; break;
            case t_float_arr: std::cout << " float*"; break;
            case t_bool_arr: std::cout << " bool*"; break;
            case t_string_arr: std::cout << " string*"; break;
            default: break;
        }
    }
    std::cout << std::endl;
    if (this->statements) {
        this->statements->debug(0);
    } else {
        std::cout << "PROTO\n";
    }
}

void Function::fold(AST* ast) {
    ast->scope->new_scope();
    for (size_t i = 0; i < this->arg_count; i++) {
        ast->scope->push_value(((ExpressionAtomic*)this->indentifiers[i])->str, new ScopeValue(this->indentifiers_mutability[i], this->indentifier_type[i]));
    }
    if (this->statements)
        this->statements->fold(ast);
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

    if (!TheFunction)
        return nullptr;
    if (!this->statements)
        return TheFunction;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*(ast->TheContext), "entry", TheFunction);
    ast->Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    ast->NamedValues.clear();
    ast->NamedValueTypes.clear();
    auto Arg = TheFunction->args().begin();
    for (size_t i = 0; i < this->indentifier_type.size(); i++) {
        llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(ast, TheFunction, std::string(Arg->getName()), Arg->getType());
        ast->Builder->CreateStore(Arg, Alloca);

        // Add arguments to variable symbol table.
        ast->NamedValues[std::string(Arg->getName())] = std::make_pair(Alloca, Arg->getType());
        ast->NamedValueTypes[std::string(Arg->getName())] = std::make_pair(this->indentifier_type[i], 1);
        Arg = std::next(Arg);
    }

    this->statements->codegen(ast);
    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
}
