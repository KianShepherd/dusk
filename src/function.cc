#include "function.hh"
#include "ast.hh"

Function::Function(std::string name, Statement* statements, AtomType type, std::vector<std::vector<std::string>> args) {
    this->name = name;
    this->statements = statements;
    this->type = type;
    this->arg_count = args.size();
    this->indentifiers = std::vector<Expression*>();
    this->indentifier_type = std::vector<AtomType>();
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

llvm::Function* Function::codegen(AST* ast) {
    // First, check for an existing function from a previous 'extern' declaration.
    llvm::Function *TheFunction = ast->TheModule->getFunction(this->name);

    if (!TheFunction) {
        // Make the function type:  double(double,double) etc.
        std::vector<llvm::Type*> func_args = std::vector<llvm::Type*>();
        for (size_t i = 0; i < this->arg_count; i++) {
            switch (this->indentifier_type[i]) {
                case t_number: func_args.push_back(llvm::Type::getInt64Ty(*(ast->TheContext))); break;
                case t_float: func_args.push_back(llvm::Type::getFloatTy(*(ast->TheContext))); break;
                case t_bool: func_args.push_back(llvm::Type::getInt1Ty(*(ast->TheContext))); break;
                case t_string: return nullptr; break;
                default: break;
            }
        }
        llvm::FunctionType *FT = nullptr;
        switch (this->type) {
            case t_number: FT = llvm::FunctionType::get(llvm::Type::getInt64Ty(*(ast->TheContext)), func_args, false); break;
            case t_float: FT = llvm::FunctionType::get(llvm::Type::getFloatTy(*(ast->TheContext)), func_args, false); break;
            case t_string: FT = nullptr; break;
            case t_null: FT = llvm::FunctionType::get(llvm::Type::getVoidTy(*(ast->TheContext)), func_args, false); break;
            case t_bool: FT = llvm::FunctionType::get(llvm::Type::getInt1Ty(*(ast->TheContext)), func_args, false); break;
            default: FT = nullptr; break;
        }

        llvm::Function *F =
            llvm::Function::Create(FT, llvm::Function::ExternalLinkage, this->name, ast->TheModule.get());

        // Set names for all arguments.
        unsigned Idx = 0;
        for (auto &Arg : F->args())
            Arg.setName(((ExpressionAtomic*)this->indentifiers[Idx++])->str);
        TheFunction = F;
    }

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*(ast->TheContext), "entry", TheFunction);
    ast->Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    ast->NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        ast->NamedValues[std::string(Arg.getName())] = &Arg;

    this->statements->codegen(ast);

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
}
