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
    std::vector<Statement*> stats;
    stats.push_back(new ReturnStatement(new ExpressionAtomic("newstruct", std::vector<Expression*>({ new ExpressionAtomic((long)this->mem_size) }))));
    auto statements = new StatementBlock(stats);
    this->member_functions.push_back(new Function(this->name, statements, t_string, std::vector<std::vector<std::string>>()));
}

void Struct::debug(size_t depth) {
    std::cout << std::endl << "Struct "<< this->name << " {" << std::endl;;

    for (int i = 0; i < this->type_idents.size(); i++) {
        std::cout << std::string(4, ' ') << this->type_idents[i] << " : " << atom_to_debug(this->types[i]) << " : " << this->gen_field_map[this->type_idents[i]] << "," << std::endl;
    }
    std::cout << std::endl;
    for (auto& f : this->member_functions) {
        std::cout << "MEMBER FUNCTION:" << std::endl;
        f->debug();
    }
    std::cout << std::endl;
    std::cout << "}" << std::endl;

}

void Struct::push_var(std::string name, AtomType type) {
    int idx = this->type_idents.size();
    this->type_idents.push_back(name);
    this->gen_field_map[name] = idx;
    int field_idx = this->types.size();
    this->types.push_back(type);
    this->llvm_types.push_back(atom_to_type(type, this->ast));
    this->field_map[name] = std::tuple<int, int>(0, field_idx);
    this->mem_size += atom_to_size(type);

}

void Struct::push_function(Function* func) {
    auto func_name = std::string(this->name).append(func->name);
    func->push_front(new ExpressionAtomic("self", true), t_struct, true);
    this->func_idents.push_back(func_name);
    this->gen_field_map[func_name] = -1;
    int field_idx = this->member_functions.size();
    this->field_map[func_name] = std::tuple<int, int>(1, field_idx);
    func->name = func_name;
    this->member_functions.push_back(func);
}

llvm::Value* Struct::codegen_functions(AST* ast) {
    for (auto& f : this->member_functions) {
        this->funcs.push_back(f->codegen(ast));
    }
    return nullptr;
}

llvm::Value* Struct::codegen(AST* ast) {
    return nullptr;
}
