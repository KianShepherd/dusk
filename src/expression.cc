#include "expression.hh"
#include "ast.hh"

ExpressionAtomic::ExpressionAtomic(long num) {
    this->number = num;
    this->type = t_number;
}

ExpressionAtomic::ExpressionAtomic(long long num) {
    this->number = num;
    this->type = t_long;
}

ExpressionAtomic::ExpressionAtomic(double num) {
    this->floating = num;
    this->type = t_float;
}

ExpressionAtomic::ExpressionAtomic(std::string str, bool is_identifier) {
    this->str = (is_identifier)?std::string(str):std::string(str.substr(1, str.length() - 2));
    this->type = (is_identifier)?t_identifier:t_string;
    this->index = nullptr;
    this->length = this->str.length();
}

ExpressionAtomic::ExpressionAtomic(std::string str, Expression* index) {
    this->str = std::string(str);
    this->type = t_identifier;
    this->index = index;
}

ExpressionAtomic::ExpressionAtomic(char character) {
    this->character = character;
    this->type = t_char;
}

ExpressionAtomic::ExpressionAtomic(bool b) {
    this->boolean = b;
    this->type = t_bool;
}

ExpressionAtomic::ExpressionAtomic() {
    this->type = t_null;
}

ExpressionAtomic::ExpressionAtomic(std::string str, std::vector<Expression*> args) {
    this->str = std::string(str);
    this->args = args;
    this->type = t_function_call;
}

ExpressionAtomic::ExpressionAtomic(AtomType type, int length, std::vector<long long> values) {
    this->type = type;
    this->length = length;
    this->int_vals = values;
}

ExpressionAtomic::ExpressionAtomic(AtomType type, int length, std::vector<double> values) {
    this->type = type;
    this->length = length;
    this->float_vals = values;
}

ExpressionAtomic::ExpressionAtomic(int length, std::vector<std::string> values) {
    this->type = t_string_arr;
    this->length = length;
    this->string_vals = values;
}

void ExpressionAtomic::debug(size_t depth) {
    switch (this->type) {
        case t_number: std::cout << std::string(depth * 4, ' ') << this->number << " : int"<< std::endl; break;
        case t_long: std::cout << std::string(depth * 4, ' ') << this->number << " : long"<< std::endl; break;
        case t_float: std::cout << std::string(depth * 4, ' ') << this->floating << " : float" << std::endl; break;
        case t_char: std::cout << std::string(depth * 4, ' ') << this->character << " : char" << std::endl; break;
        case t_string: std::cout << std::string(depth * 4, ' ') << this->str << " - len = " << this->str.length() << " : str" << std::endl; break;
        case t_identifier: {
            std::cout << std::string(depth * 4, ' ') << this->str << " - len = " << this->str.length() << " : identifier";
            if (this->index) {
                std:: cout << " [ ";
                this->index->debug(depth + 1);
                std::cout << std::string(depth * 4, ' ') << " ] ";
            }
            std::cout << std::endl;
            break;
        }
        case t_null: std::cout << std::string(depth * 4, ' ') << "null : null" << std::endl; break;
        case t_bool: std::cout << std::string(depth * 4, ' ') << ((this->boolean)?"True : bool":"False : bool") << std::endl; break;
        case t_function_call: std::cout << std::string(depth * 4, ' ') << "call " << this->str << " : " << this->args.size() << " args" << std::endl; for (size_t i = 0; i < this->args.size(); i++) this->args[i]->debug(depth + 1); break;
        case t_bool_arr: {
            std::cout << std::string(depth * 4, ' ') << "bool array : len " << this->length << " { ";
            for (auto v: this->int_vals) {
                std::cout << v << " ";
            }
            std::cout << "}" << std::endl;
            break;
        }
        case t_number_arr: {
            std::cout << std::string(depth * 4, ' ') << "int array : len " << this->length << " { ";
            for (auto v: this->int_vals) {
                std::cout << v << " ";
            }
            std::cout << "}" << std::endl;
            break;
        }
        case t_float_arr: {
            std::cout << std::string(depth * 4, ' ') << "float array : len " << this->length << " { ";
            for (auto v: this->int_vals) {
                std::cout << v << " ";
            }
            std::cout << "}" << std::endl;
            break;
        }
        case t_string_arr: {
            std::cout << std::string(depth * 4, ' ') << "string array : len " << this->length << " { ";
            for (auto v: this->string_vals) {
                std::cout << v << " ";
            }
            std::cout << "}" << std::endl;
            break;
        }
        default: std::cout << "Unknown atomic type" << std::endl; break;
    }
}

AtomType ExpressionAtomic::get_atomic_type(AST* ast) {
    if (this->type == t_identifier) {
        AtomType value = ast->scope->get_value(this->str);
        if (value == t_null)
            ast->push_err("Attempted to lookup unknown identifier.");
        return value;
    } else if (this->type == t_function_call) {
        for (auto &func : ast->func_definitions) {
            if (this->str.compare(std::get<0>(func)) == 0)
                return std::get<1>(func);
        }
    }
    return this->type;
}

AtomType ExpressionAtomic::get_atomic_type_keep_identifier(AST* ast) {
    if (this->type == t_function_call) {
        for (auto &func : ast->func_definitions) {
            if (this->str.compare(std::get<0>(func)) == 0)
                return std::get<1>(func);
        }
    }
    return this->type;
}

Expression* ExpressionAtomic::fold(AST* ast) {
    return (Expression*)this;
}

llvm::Value* ExpressionAtomic::codegen(AST* ast, AtomType type) {
    switch (this->type) {
        case t_number: {
            if (type == t_long) {
                return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(128, this->number, true));
            } else if (type == t_char) {
                return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, this->number, true));
            }
            return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, this->number, true));
        }
        case t_long: return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(128, this->number, true));
        case t_char: return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, this->character, true));
        case t_float: return llvm::ConstantFP::get(*(ast->TheContext), llvm::APFloat(this->floating));
        case t_string: {
            char c ;
            std::stringstream s(this->str);
            std::stringstream s2;

            while (s.good()) {
                c = s.get() ;
                if (!s.good()) break ;
                if (c == '\\' && s.get() == 'n') c = '\n' ;
                s2 << c ;
            }
            this->str = s2.str();

            auto atom_type = llvm::Type::getInt8Ty(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->str.size() + 1);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("s"), ast->Builder->GetInsertBlock());
            for (size_t i = 0; i < this->str.size(); i++) {
                auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, this->str.data()[i],true)), ptr);
            }
            auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, this->str.size());
            ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, 0,true)), ptr);
            return array;
        }
        case t_identifier: {
            // Look this variable up in the function.
            llvm::Value *V = ast->NamedValues[this->str].first;
            if (!V)
                return ast->LogErrorV("Unknown variable name");
            if (!this->index) {
                return ast->Builder->CreateLoad(ast->NamedValues[this->str].second, V, this->str.c_str());
            } else {

                llvm::Type* atom_type = nullptr;
                AtomType vec_type = ast->NamedValueTypes[this->str].first;
                if (vec_type == t_float_arr) {
                    atom_type = llvm::Type::getDoubleTy(*(ast->TheContext));
                } else if (vec_type == t_number_arr) {
                    atom_type = llvm::Type::getInt64Ty(*(ast->TheContext));
                } else if (vec_type == t_bool_arr) {
                    atom_type = llvm::Type::getInt1Ty(*(ast->TheContext));
                } else if (vec_type == t_string_arr) {
                    atom_type = llvm::Type::getInt8PtrTy(*(ast->TheContext));
                }
                auto index_val = this->index->codegen(ast, t_number);
                auto index_vec = std::vector<llvm::Value*>();
                index_vec.push_back(index_val);

                auto Vd = ast->Builder->CreateLoad(ast->NamedValues[this->str].second, V, this->str.c_str());

                auto ptr = ast->Builder->CreateGEP(atom_type, Vd, index_vec);
                return ast->Builder->CreateLoad(atom_type, ptr, this->str.c_str());
            }
        }
        case t_null: return nullptr;
        case t_bool: return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, ((this->boolean)?1:0), false));
        case t_function_call: {
            llvm::Function *CalleeF = ast->TheModule->getFunction(this->str);
            if (!CalleeF)
                return ast->LogErrorV("Unknown function referenced");

            // If argument mismatch error.
            if (CalleeF->arg_size() != this->args.size() && !CalleeF->isVarArg())
                return ast->LogErrorV("Incorrect # arguments passed");

            std::vector<AtomType> arg_type;
            for (auto &func : ast->func_definitions) {
                if (this->str.compare(std::get<0>(func)) == 0) {
                    arg_type = std::get<2>(func);
                    break;
                }
            }

            std::vector<llvm::Value*> ArgsV;
            for (unsigned i = 0, e = this->args.size(); i != e; ++i) {
                if (this->str.compare("print") != 0) {
                    ArgsV.push_back(this->args[i]->codegen(ast, arg_type[i]));
                } else {
                    ArgsV.push_back(this->args[i]->codegen(ast, t_null));
                }
                if (!ArgsV.back())
                    return nullptr;
            }
            auto ret = ast->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
            return ret;
        }
        case t_bool_arr: {
            auto atom_type = llvm::Type::getInt1Ty(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("ba"), ast->Builder->GetInsertBlock());
            for (size_t i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, (bool)this->int_vals[i],false)), ptr);
            }
            return array;
        }
        case t_number_arr: {
            auto atom_type = llvm::Type::getInt64Ty(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*(ast->TheContext)), 1), llvm::Twine("na"), ast->Builder->GetInsertBlock());
            for (size_t i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_64(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, (long)this->int_vals[i],true)), ptr);
            }
            return array;
        }
        case t_float_arr: {
            auto atom_type = llvm::Type::getDoubleTy(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("fa"), ast->Builder->GetInsertBlock());
            for (size_t i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantFP::get(*(ast->TheContext), llvm::APFloat((double)this->float_vals[i])), ptr);
            }
            return array;
        }
        case t_string_arr: {
            auto atom_type = llvm::Type::getInt8PtrTy(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("sa"), ast->Builder->GetInsertBlock());
            for (size_t i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, i);

                char c;
                std::stringstream s(this->string_vals[i]);
                std::stringstream s2;
                
                while (s.good()) {
                    c = s.get() ;
                    if (!s.good()) break ;
                    if (c == '\\' && s.get() == 'n') c = '\n' ;
                    s2 << c ;
                }
                this->string_vals[i] = s2.str().substr(1, s2.str().length() - 2);
                auto s_atom_type = llvm::Type::getInt8Ty(*(ast->TheContext));
                auto s_type = llvm::ArrayType::get(s_atom_type, this->string_vals[i].size() + 1);

                auto s_array = new llvm::AllocaInst(s_type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("s"), ast->Builder->GetInsertBlock());
                for (size_t j = 0; j < this->string_vals[i].size(); j++) {
                    auto s_ptr = ast->Builder->CreateConstGEP1_32(s_atom_type, s_array, j);
                    ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, this->string_vals[i].data()[j],true)), s_ptr);
                }
                auto s_ptr = ast->Builder->CreateConstGEP1_32(s_atom_type, s_array, this->string_vals[i].size());
                ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, 0,true)), s_ptr);

                ast->Builder->CreateStore(s_array, ptr);
            }
            return array;

        }
        default: return nullptr; break;
    }
}

BinaryExpression::BinaryExpression(Expression* l, Expression* r, Operators t) {
    this->lhs = l;
    this->rhs = r;
    this->type = t;
}

void BinaryExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "binary ";
    switch (this->type) {
        case op_add: std::cout << "+"; break;
        case op_sub: std::cout << "-"; break;
        case op_mul: std::cout << "*"; break;
        case op_div: std::cout << "/"; break;
        case op_equal: std::cout << "=="; break;
        case op_not_equal: std::cout << "!="; break;
        case op_greater: std::cout << ">"; break;
        case op_less: std::cout << "<"; break;
        case op_greater_equal: std::cout << ">="; break;
        case op_less_equal: std::cout << "<="; break;
        case op_and: std::cout << "and"; break;
        case op_or: std::cout << "or"; break;
    }
    std::cout << std::endl;
    this->lhs->debug(depth + 1);
    this->rhs->debug(depth + 1);
    
}

Expression* BinaryExpression::fold(AST* ast) {
    return (Expression*)this;
}

llvm::Value* BinaryExpression::codegen(AST* ast, AtomType type) {
    if (this->lhs->get_atomic_type_keep_identifier(ast) == t_identifier) {
        if (ast->NamedValues[((ExpressionAtomic*)this->lhs)->str].second->isIntegerTy(8)) {
            type = t_char;
        } else if (ast->NamedValues[((ExpressionAtomic*)this->lhs)->str].second->isIntegerTy(64)) {
            type = t_number;
        } else if (ast->NamedValues[((ExpressionAtomic*)this->lhs)->str].second->isIntegerTy(128)) {
            type = t_long;
        }
    } else if (this->rhs->get_atomic_type_keep_identifier(ast) == t_identifier) {
        if (ast->NamedValues[((ExpressionAtomic*)this->rhs)->str].second->isIntegerTy(8)) {
            type = t_char;
        } else if (ast->NamedValues[((ExpressionAtomic*)this->rhs)->str].second->isIntegerTy(64)) {
            type = t_number;
        } else if (ast->NamedValues[((ExpressionAtomic*)this->rhs)->str].second->isIntegerTy(128)) {
            type = t_long;
        }
    }
    llvm::Value *L = this->lhs->codegen(ast, type);
    llvm::Value *R = this->rhs->codegen(ast, type);

    if (!L || !R)
        return nullptr;
    
    if (L->getType() == llvm::Type::getDoubleTy(*(ast->TheContext)) || R->getType() == llvm::Type::getDoubleTy(*(ast->TheContext))) {
        switch (this->type) {
            case op_add:            return ast->Builder->CreateFAdd(L, R);
            case op_sub:            return ast->Builder->CreateFSub(L, R);
            case op_mul:            return ast->Builder->CreateFMul(L, R);
            case op_div:            return ast->Builder->CreateFDiv(L, R);
            case op_equal:          return ast->Builder->CreateFCmpOEQ(L, R);
            case op_not_equal:      return ast->Builder->CreateFCmpONE(L, R);
            case op_greater:        return ast->Builder->CreateFCmpOGT(L, R);
            case op_less:           return ast->Builder->CreateFCmpOLT(L, R);
            case op_greater_equal:  return ast->Builder->CreateFCmpOGE(L, R);
            case op_less_equal:     return ast->Builder->CreateFCmpOLE(L, R);
            case op_and:            return nullptr;
            case op_or:             return nullptr;
        }
    } else if (L->getType() == llvm::Type::getInt64Ty(*(ast->TheContext)) || R->getType() == llvm::Type::getInt64Ty(*(ast->TheContext))) {
        switch (this->type) {
            case op_add:            return ast->Builder->CreateAdd(L, R);
            case op_sub:            return ast->Builder->CreateSub(L, R);
            case op_mul:            return ast->Builder->CreateMul(L, R);
            case op_div:            return ast->Builder->CreateSDiv(L, R);
            case op_equal:          return ast->Builder->CreateICmpEQ(L, R);
            case op_not_equal:      return ast->Builder->CreateICmpNE(L, R);
            case op_greater:        return ast->Builder->CreateICmpSGT(L, R);
            case op_less:           return ast->Builder->CreateICmpSLT(L, R);
            case op_greater_equal:  return ast->Builder->CreateICmpSGE(L, R);
            case op_less_equal:     return ast->Builder->CreateICmpSLE(L, R);
            case op_and:            return nullptr;
            case op_or:             return nullptr;
        }
    } else if (L->getType() == llvm::Type::getInt128Ty(*(ast->TheContext)) || R->getType() == llvm::Type::getInt128Ty(*(ast->TheContext))) {
        switch (this->type) {
            case op_add:            return ast->Builder->CreateAdd(L, R);
            case op_sub:            return ast->Builder->CreateSub(L, R);
            case op_mul:            return ast->Builder->CreateMul(L, R);
            case op_div:            return ast->Builder->CreateSDiv(L, R);
            case op_equal:          return ast->Builder->CreateICmpEQ(L, R);
            case op_not_equal:      return ast->Builder->CreateICmpNE(L, R);
            case op_greater:        return ast->Builder->CreateICmpSGT(L, R);
            case op_less:           return ast->Builder->CreateICmpSLT(L, R);
            case op_greater_equal:  return ast->Builder->CreateICmpSGE(L, R);
            case op_less_equal:     return ast->Builder->CreateICmpSLE(L, R);
            case op_and:            return nullptr;
            case op_or:             return nullptr;
        }
    } else if (L->getType() == llvm::Type::getInt8Ty(*(ast->TheContext)) || R->getType() == llvm::Type::getInt8Ty(*(ast->TheContext))) {
        switch (this->type) {
            case op_add:            return ast->Builder->CreateAdd(L, R);
            case op_sub:            return ast->Builder->CreateSub(L, R);
            case op_mul:            return ast->Builder->CreateMul(L, R);
            case op_div:            return ast->Builder->CreateSDiv(L, R);
            case op_equal:          return ast->Builder->CreateICmpEQ(L, R);
            case op_not_equal:      return ast->Builder->CreateICmpNE(L, R);
            case op_greater:        return ast->Builder->CreateICmpSGT(L, R);
            case op_less:           return ast->Builder->CreateICmpSLT(L, R);
            case op_greater_equal:  return ast->Builder->CreateICmpSGE(L, R);
            case op_less_equal:     return ast->Builder->CreateICmpSLE(L, R);
            case op_and:            return nullptr;
            case op_or:             return nullptr;
        }
    } else if (L->getType() == llvm::Type::getInt1Ty(*(ast->TheContext)) || R->getType() == llvm::Type::getInt1Ty(*(ast->TheContext))) {
        switch (this->type) {
            case op_add:            return nullptr;
            case op_sub:            return nullptr;
            case op_mul:            return nullptr;
            case op_div:            return nullptr;
            case op_equal:          return nullptr;
            case op_not_equal:      return nullptr;
            case op_greater:        return nullptr;
            case op_less:           return nullptr;
            case op_greater_equal:  return nullptr;
            case op_less_equal:     return nullptr;
            case op_and:            return ast->Builder->CreateLogicalAnd(L, R);
            case op_or:             return ast->Builder->CreateLogicalOr(L, R);
        }
    }
    return nullptr;
}


AtomType BinaryExpression::get_atomic_type(AST* ast) {
    if (this->lhs->get_atomic_type(ast) != this->rhs->get_atomic_type(ast)) {
        ast->push_err("Both operands of a binary operator must be of same type");
    }
    return this->lhs->get_atomic_type(ast);
}

AtomType BinaryExpression::get_atomic_type_keep_identifier(AST* ast) {
    if (this->lhs->get_atomic_type(ast) != this->rhs->get_atomic_type(ast)) {
        ast->push_err("Both operands of a binary operator must be of same type");
    }
    return this->lhs->get_atomic_type(ast);
}

UnaryExpression::UnaryExpression(Expression* expr, std::string op) {
    this->operand = expr;
    this->op = op;
}

void UnaryExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "unary " << this->op << std::endl;
    this->operand->debug(depth + 1);
    
}

Expression* UnaryExpression::fold(AST* ast) {
    return (Expression*)this;
}

llvm::Value* UnaryExpression::codegen(AST* ast, AtomType type) {
    llvm::Value *L = this->operand->codegen(ast, type);

    if (this->op.compare("()") == 0) {
        return L;
    }
    if (L->getType() == llvm::Type::getDoubleTy(*(ast->TheContext))) {
        if (this->op.compare("-") == 0) {
            return ast->Builder->CreateFMul(L, llvm::ConstantFP::get(*(ast->TheContext), llvm::APFloat(-1.0)));
        }
    } else if (L->getType() == llvm::Type::getInt64Ty(*(ast->TheContext))) {
        if (this->op.compare("-") == 0) {
            return ast->Builder->CreateMul(L, llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, -1, true)));
        }
    } else if (L->getType() == llvm::Type::getInt128Ty(*(ast->TheContext))) {
        if (this->op.compare("-") == 0) {
            return ast->Builder->CreateMul(L, llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(128, -1, true)));
        }
    } else if (L->getType() == llvm::Type::getInt8Ty(*(ast->TheContext))) {
        if (this->op.compare("-") == 0) {
            return ast->Builder->CreateMul(L, llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(8, -1, true)));
        }
    } else if (L->getType() == llvm::Type::getInt1Ty(*(ast->TheContext))) {
        if (this->op.compare("!") == 0) {
            return ast->Builder->CreateXor(L, llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, 1, false)));
        }
    }
    return nullptr;
}

AtomType UnaryExpression::get_atomic_type(AST* ast) {
    return this->operand->get_atomic_type(ast);
}

AtomType UnaryExpression::get_atomic_type_keep_identifier(AST* ast) {
    return this->operand->get_atomic_type(ast);
}

AssignmentExpression::AssignmentExpression(Expression* identifier, Expression* value) {
    this->identifier = identifier;
    this->value = value;
    this->index = 0;
    this->is_arr = false;
}

AssignmentExpression::AssignmentExpression(Expression* identifier, Expression* value, Expression* index) {
    this->identifier = identifier;
    this->value = value;
    this->index = index;
    this->is_arr = true;
}

void AssignmentExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << ((ExpressionAtomic*)this->identifier)->str;
    if (this->index) {
        std::cout << std::string(depth * 4, ' ') << "[";
        this->index->debug(depth);
        std::cout << std::string(depth * 4, ' ') << "]";
    }
    std::cout << " = " << std::endl;
    this->value->debug(depth + 1);
}

Expression* AssignmentExpression::fold(AST* ast) {
    this->value = this->value->fold(ast);
    ast->scope->update_value(ast, ((ExpressionAtomic*)this->identifier)->str, new ScopeValue(true, ((ExpressionAtomic*)this->value)->get_atomic_type(ast)));
    return (Expression*)this;
}

llvm::Value* AssignmentExpression::codegen(AST* ast, AtomType type) {
    AtomType t = t_null;
    if (ast->NamedValues[((ExpressionAtomic*)this->identifier)->str].second->isIntegerTy(8)) {
        t = t_char;
    } else if (ast->NamedValues[((ExpressionAtomic*)this->identifier)->str].second->isIntegerTy(64)) {
        t = t_number;
    } else if (ast->NamedValues[((ExpressionAtomic*)this->identifier)->str].second->isIntegerTy(128)) {
        t = t_long;
    }
    llvm::Value *Val = this->value->codegen(ast, t);
    if (!Val)
        return nullptr;

    llvm::Value *Variable = ast->NamedValues[((ExpressionAtomic*)this->identifier)->str].first;
    if (!Variable)
        return ast->LogErrorV("Unknown variable name");
    if (!this->is_arr) {
        ast->Builder->CreateStore(Val, Variable);
    } else {
        llvm::Type* atom_type = nullptr;
        AtomType vec_type = ast->NamedValueTypes[((ExpressionAtomic*)this->identifier)->str].first;
        if (vec_type == t_float_arr) {
            atom_type = llvm::Type::getDoubleTy(*(ast->TheContext));
        } else if (vec_type == t_number_arr) {
            atom_type = llvm::Type::getInt64Ty(*(ast->TheContext));
        } else if (vec_type == t_bool_arr) {
            atom_type = llvm::Type::getInt1Ty(*(ast->TheContext));
        } else if (vec_type == t_string_arr) {
            atom_type = llvm::Type::getInt8PtrTy(*(ast->TheContext));
        }
        auto index_val = this->index->codegen(ast, t_number);
        auto index_vec = std::vector<llvm::Value*>();
        index_vec.push_back(index_val);

        auto Arr = ast->Builder->CreateLoad(llvm::Type::getInt64PtrTy(*(ast->TheContext)), Variable);
        auto ptr = ast->Builder->CreateGEP(atom_type, Arr, index_vec);

        ast->Builder->CreateStore(Val, ptr);
    }
    return Val;
}

AtomType AssignmentExpression::get_atomic_type(AST* ast) {
    return this->value->get_atomic_type(ast);
}

AtomType AssignmentExpression::get_atomic_type_keep_identifier(AST* ast) {
    return this->value->get_atomic_type(ast);
}

BreakExpression::BreakExpression() {

}

void BreakExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "BREAK" << std::endl;
}

Expression* BreakExpression::fold(AST* ast) {
    return (Expression*)this;
}

llvm::Value* BreakExpression::codegen(AST* ast, AtomType type) {
    return nullptr;
}

AtomType BreakExpression::get_atomic_type(AST* ast) {
    return t_null;
}

AtomType BreakExpression::get_atomic_type_keep_identifier(AST* ast) {
    return t_null;
}
