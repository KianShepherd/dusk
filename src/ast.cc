#include "ast.hh"

std::string type_string(AtomType type) {
    switch (type) {
        case t_number: return std::string("int");
        case t_long: return std::string("long");
        case t_float: return std::string("float");
        case t_char: return std::string("char");
        case t_string: return std::string("string");
        case t_identifier: {
            // TODO
            return std::string("");
        }
        case t_bool: return std::string("bool");
        case t_function_call: {
            // TODO
            return std::string("");
        }
        case t_bool_arr: return std::string("bool*");
        case t_number_arr: return std::string("int*");
        case t_float_arr: return std::string("float*");
        case t_string_arr: return std::string("string*");
        case t_get_struct: {
            // TODO
            return std::string("");
        }
        default: return std::string("");
    }
}

AST::AST() {
    this->functions = std::vector<Function*>();
    this->error = false;
    this->err = std::stringstream();
    this->scope = new ScopeFrame();
    this->NamedValues = new CodegenScopeFrame();
    this->NamedValues = this->NamedValues->new_scope();

    this->TheContext = std::make_unique<llvm::LLVMContext>();
    this->TheModule = std::make_unique<llvm::Module>("dusk", *(this->TheContext));

    // Create a new builder for the module.
    this->Builder = std::make_unique<llvm::IRBuilder<>>(*(this->TheContext));
    this->stdlib();
}

void AST::push_function(Function* function) {
    this->functions.push_back(function);
    this->func_map[function->name] = function;
}

void AST::push_include(std::string file) {
    this->includes.push_back(file);
}

void AST::push_requires(std::string file) {
    this->require.push_back(file);
}

void AST::push_struct(Struct* s) {
    if (!this->struct_map[s->name]) {
        this->structs.push_back(s);
        this->struct_map[s->name] = s;
    } else {
        for (int i = 0; i < (int)s->type_idents.size(); i++) {
            if (s->struct_var_map.count(s->type_idents[i])) {
                this->struct_map[s->name]->push_var(s->type_idents[i], s->types[i], s->struct_var_map[s->type_idents[i]]);
            } else {
                this->struct_map[s->name]->push_var(s->type_idents[i], s->types[i]);
            }
        }
        for (int i = 0; i < (int)s->member_functions.size(); i++) {
            this->struct_map[s->name]->push_function_pre(s->member_functions[i]);
        }
        for (int i = 0; i < (int)s->constructors.size(); i++) {
            this->struct_map[s->name]->push_constructor(s->constructors[i]);
        }
    }
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
    std::cout << "----- DEBUG AST -----" << std::endl;
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->functions[i]->debug();
    }
    for (int i = 0; i < (int)this->structs.size(); i++) {
        this->structs[i]->debug(0);
    }
    std::cout << "----- DEBUG AST -----" << std::endl;
}

void AST::static_checking() {
    for (auto& s : this->structs) {
        s->finalize();
        for (int i = 0; i < (int)s->member_functions.size(); i++) {
            this->func_map[s->member_functions[i]->name] = s->member_functions[i];
            this->functions.push_back(s->member_functions[i]);
        }
    }
    bool found_entrypoint = false;
    for (int i = 0; i < (int)this->functions.size(); i++) {
        auto meta = this->functions[i]->get_meta();
        this->func_definitions.push_back(meta);
        if (std::get<0>(meta).compare("main") == 0) {
            if (found_entrypoint)
                this->push_err("Only one main function may be defined within an executable.");
            found_entrypoint = true;
        }
    }
    /*
    for (int i = 0; i < (int)this->structs.size(); i++) {
        for (int j = 0; j < (int)this->structs[i]->member_functions.size(); j++) {
            this->structs[i]->member_functions[j]->fold(this);
        }
    }
    */
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->functions[i]->fold(this);
    }
    if (!found_entrypoint)
        this->push_err("No entrypoint found, you must declare a main function.");
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
    std::vector<llvm::Type*> func_args = std::vector<llvm::Type*>();
    func_args.push_back(llvm::Type::getInt8PtrTy(*(this->TheContext)));
    llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getInt64Ty(*(this->TheContext)), func_args, true),
        llvm::Function::ExternalLinkage,
        "print",
        this->TheModule.get()
    );
    func_args = std::vector<llvm::Type*>();
    func_args.push_back(llvm::Type::getInt8PtrTy(*(this->TheContext)));
    llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getInt64Ty(*(this->TheContext)), func_args, true),
        llvm::Function::ExternalLinkage,
        "println",
        this->TheModule.get()
    );
}

void AST::codegen(char debug, bool optimizations, std::string outfile) {
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
    // Create the analysis managers.
    llvm::LoopAnalysisManager LAM;
    llvm::FunctionAnalysisManager FAM;
    llvm::CGSCCAnalysisManager CGAM;
    llvm::ModuleAnalysisManager MAM;

    // Create the new pass manager builder.
    // Take a look at the PassBuilder constructor parameters for more
    // customization, e.g. specifying a TargetMachine or various debugging
    // options.
    llvm::PassBuilder PB;

    // Register all the basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    // Create the pass manager.
    llvm::ModulePassManager MPM = PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O3);

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    this->TheModule->setDataLayout(TargetMachine->createDataLayout());
    this->TheModule->setTargetTriple(TargetTriple);
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->func_definitions.push_back(this->functions[i]->get_meta());
        // Forward declare all the function definitions
        this->functions[i]->codegen_proto(this);
    }
    for (int i = 0; i < (int)this->structs.size(); i++) {
        for (int j = 0; j < (int)this->structs[i]->member_functions.size(); j++) {
            this->func_definitions.push_back(this->structs[i]->member_functions[j]->get_meta());
            // Forward declare all the function definitions
            this->structs[i]->member_functions[j]->codegen_proto(this);
        }
    }
    // Codegen all of the actual functions
    for (int i = 0; i < (int)this->functions.size(); i++) {
        this->functions[i]->codegen(this);
    }
    if (debug) {
        std::cout << "===== DEBUG IR ======" << std::endl;
        this->TheModule->dump();
    }

    if (optimizations) {
        // Optimize the IR!
        MPM.run(*(this->TheModule), MAM);

        if (debug) {
            std::cout << "===== OPTIMIZE ======" << std::endl;
            this->TheModule->dump();
        }
    }
    if (debug) {
        std::cout << "===== DEBUG IR ======" << std::endl;
    }

    auto Filename = outfile.data();
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

ScopeValue::ScopeValue(bool mut, AtomType type, std::string struct_name) {
    this->mut = mut;
    this->type = type;
    this->struct_name = struct_name;
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
        if (iter != scope->variables.end()) {
            if (!scope->variables[identifier]->mut) {
                ast->push_err("Attempted to mutate a static variable.\n");
                ast->push_err(identifier);
                ast->push_err("\n");
            }
            break;
        }
        scope = scope->prev_frame;
    }
    if (scope == nullptr)
        ast->push_err("Undeclared variable found.");
}

AtomType ScopeFrame::get_value(std::string identifier) {
    auto iter = this->variables.find(identifier);
    if (iter != this->variables.end() ) {
        return iter->second->type;
    }
    if (this->prev_frame == nullptr)
        return t_null;
    return this->prev_frame->get_value(identifier);
}

std::string ScopeFrame::get_value_struct(std::string identifier) {
    auto iter = this->variables.find(identifier);
    if (iter != this->variables.end() ) {
        return iter->second->struct_name;
    }
    if (this->prev_frame == nullptr)
        return std::string("");
    return this->prev_frame->get_value_struct(identifier);
}

CodegenScopeFrame::CodegenScopeFrame() {
    this->prev_frame = nullptr;
    this->next_frame = nullptr;
    this->NamedValues = std::map<std::string, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long>>();
}

CodegenScopeFrame::CodegenScopeFrame(CodegenScopeFrame* prev) {
    this->prev_frame = prev;
    this->next_frame = nullptr;
    this->NamedValues = std::map<std::string, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long>>();
}

CodegenScopeFrame* CodegenScopeFrame::new_scope() {
    CodegenScopeFrame* next = new CodegenScopeFrame(this);
    this->next_frame = next;
    return next;
}

void CodegenScopeFrame::push_value(std::string identifier, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long>
 value) {
    this->NamedValues[identifier] = value;
}

void CodegenScopeFrame::update_value(std::string identifier, std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long> value) {
    CodegenScopeFrame* scope = this;
    while (scope != nullptr) {
        auto iter = scope->NamedValues.find(identifier);
        if (iter != scope->NamedValues.end()) {
            scope->NamedValues[identifier] = value;
            break;
        }
        scope = scope->prev_frame;
    }
}

std::tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long> CodegenScopeFrame::get_value(std::string identifier) {
    auto iter = this->NamedValues.find(identifier);
    if (iter != this->NamedValues.end())
        return this->NamedValues[identifier];
    if (this->prev_frame == nullptr) {
        return std::make_tuple<llvm::AllocaInst*, llvm::Type*, AtomType, long>(nullptr, nullptr, t_null, 0);
    }
    return this->prev_frame->get_value(identifier);
}
