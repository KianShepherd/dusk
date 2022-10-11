#include "ast.hh"

AST::AST() {
    this->functions = std::vector<Function*>();
    this->error = false;
    this->err = std::stringstream();
    this->scope = new ScopeFrame();

    this->TheContext = std::make_unique<llvm::LLVMContext>();
    this->TheModule = std::make_unique<llvm::Module>("dusk", *(this->TheContext));

    // Create a new builder for the module.
    this->Builder = std::make_unique<llvm::IRBuilder<>>(*(this->TheContext));
    TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(this->TheModule.get());

    // Promote allocas to registers.
    TheFPM->add(llvm::createPromoteMemoryToRegisterPass());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    TheFPM->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add(llvm::createCFGSimplificationPass());

    TheFPM->doInitialization();
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
        this->scope->next_frame = nullptr;
    }
}

void AST::stdlib() {
    std::vector<std::tuple<std::string, llvm::FunctionType*, std::vector<std::string>>> functions;
    functions.push_back(
        std::make_tuple(
            std::string("printi"),
            llvm::FunctionType::get(llvm::Type::getInt64Ty(*(this->TheContext)), std::vector<llvm::Type*>{llvm::Type::getInt64Ty(*(this->TheContext))}, false),
            std::vector<std::string>{std::string("a")}
        )
    );
    functions.push_back(
        std::make_tuple(
            std::string("printd"),
            llvm::FunctionType::get(llvm::Type::getInt64Ty(*(this->TheContext)), std::vector<llvm::Type*>{llvm::Type::getDoubleTy(*(this->TheContext))}, false),
            std::vector<std::string>{std::string("a")}
        )
    );

    for (auto &func : functions) {

        llvm::Function *F = llvm::Function::Create(std::get<1>(func), llvm::Function::ExternalLinkage, std::get<0>(func), TheModule.get());

        unsigned Idx = 0;
        for (auto &Arg : F->args()) {
            Arg.setName(std::get<2>(func)[Idx++]);
        }
    }
}

void AST::codegen(char debug) {
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!Target) {
        llvm::errs() << Error;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    this->TheModule->setDataLayout(TargetMachine->createDataLayout());
    this->TheModule->setTargetTriple(TargetTriple);
    this->stdlib();
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->functions[i]->codegen(this);
    }
    if (debug)
        this->TheModule->dump();

    auto Filename = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);
    if (EC) {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }
    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;
    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return;
    }
    pass.run(*(this->TheModule));
    dest.flush();
}

llvm::Value* AST::LogErrorV(const char *Str) {
    this->push_err(std::string(Str));
    return nullptr;
}

ScopeValue::ScopeValue(Expression* value, bool mut) {
    this->value = value;
    this->mut = mut;
}

ScopeFrame::ScopeFrame() {
    this->prev_frame = nullptr;
    this->next_frame = nullptr;
    this->variables = std::map<std::string, ScopeValue*>();
}

ScopeFrame::ScopeFrame(ScopeFrame* prev) {
    this->prev_frame = prev;
    this->next_frame = nullptr;
    this->variables = std::map<std::string, ScopeValue*>();
}

ScopeFrame* ScopeFrame::new_scope() {
    ScopeFrame* next = new ScopeFrame(this);
    this->next_frame = next;
    return next;
}

void ScopeFrame::push_value(std::string identifier, ScopeValue* value) {
    this->variables[identifier] = value;
}

void ScopeFrame::update_value(AST* ast, std::string identifier, ScopeValue* value) {
    ScopeFrame* scope = this;
    while (scope != nullptr) {
        auto iter = scope->variables.find(identifier);
        if (iter != scope->variables.end() ) {
            scope->variables[identifier] = value;
            break;
        }
        scope = scope->prev_frame;
    }
    if (scope == nullptr)
        ast->push_err("Undeclared variable found.");
}

Expression* ScopeFrame::get_value(std::string identifier) {
    auto iter = this->variables.find(identifier);
    if (iter != this->variables.end() ) {
        return iter->second->value;
    }
    if (this->prev_frame == nullptr)
        return nullptr;
    return this->prev_frame->get_value(identifier);
}
