#include "expression.hh"
#include "ast.hh"

ExpressionAtomic::ExpressionAtomic(long long num) {
    this->number = num;
    this->type = t_number;
}

ExpressionAtomic::ExpressionAtomic(double num) {
    this->floating = num;
    this->type = t_float;
}

ExpressionAtomic::ExpressionAtomic(std::string str, bool is_identifier) {
    this->str = (is_identifier)?std::string(str):std::string(str.substr(1, str.length() - 2));
    this->type = (is_identifier)?t_identifier:t_string;
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

void ExpressionAtomic::debug(size_t depth) {
    switch (this->type) {
        case t_number: std::cout << std::string(depth * 4, ' ') << this->number << " : int"<< std::endl; break;
        case t_float: std::cout << std::string(depth * 4, ' ') << this->floating << " : float" << std::endl; break;
        case t_string: std::cout << std::string(depth * 4, ' ') << this->str << " - len = " << this->str.length() << " : str" << std::endl; break;
        case t_identifier: std::cout << std::string(depth * 4, ' ') << this->str << " - len = " << this->str.length() << " : identifier" << std::endl; break;
        case t_null: std::cout << std::string(depth * 4, ' ') << "null : null" << std::endl; break;
        case t_bool: std::cout << std::string(depth * 4, ' ') << ((this->boolean)?"True : bool":"False : bool") << std::endl; break;
        case t_function_call: std::cout << std::string(depth * 4, ' ') << "call " << this->str << " : " << this->args.size() << " args" << std::endl; for (size_t i = 0; i < this->args.size(); i++) this->args[i]->debug(depth + 1); break;
        default: std::cout << "Unknown atomic type" << std::endl; break;
    }
}

Expression* ExpressionAtomic::fold(AST* ast) {
    if (this->type == t_identifier) {
        Expression* value = ast->scope->get_value(this->str);
        if (value == nullptr) {
            ast->push_err("Attempted to lookup unknown identifier.");
        } else if (value->get_type() == t_atomic) {
            return (Expression*)value;
        }
    }
    return (Expression*)this;
}

llvm::Value* ExpressionAtomic::codegen(AST* ast) {
    switch (this->type) {
        case t_number: return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, (double)this->number, true));
        case t_float: return llvm::ConstantFP::get(*(ast->TheContext), llvm::APFloat(this->floating));
        case t_string: return nullptr;
        case t_identifier: {
            // Look this variable up in the function.
            llvm::Value *V = ast->NamedValues[this->str];
            if (!V)
                return ast->LogErrorV("Unknown variable name");
            return V;
        }
        case t_null: return nullptr;
        case t_bool: return llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, ((this->boolean)?1:0), true));
        case t_function_call: {
            llvm::Function *CalleeF = ast->TheModule->getFunction(this->str);
            if (!CalleeF)
            return ast->LogErrorV("Unknown function referenced");

            // If argument mismatch error.
            if (CalleeF->arg_size() != this->args.size())
                return ast->LogErrorV("Incorrect # arguments passed");

            std::vector<llvm::Value*> ArgsV;
            for (unsigned i = 0, e = this->args.size(); i != e; ++i) {
                ArgsV.push_back(this->args[i]->codegen(ast));
                if (!ArgsV.back())
                    return nullptr;
            }

            return ast->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
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
    this->lhs = this->lhs->fold(ast);
    this->rhs = this->rhs->fold(ast);
    if (this->lhs->get_type() == t_atomic && this->rhs->get_type() == t_atomic) {
        if (((ExpressionAtomic*)this->lhs)->get_atomic_type() != ((ExpressionAtomic*)this->rhs)->get_atomic_type()) {
            ast->push_err("Can not apply binary expression to atomics of two different types.");
            return this;
        }
        if (((ExpressionAtomic*)this->lhs)->type == t_number) {
            switch (this->type) {
                case op_add:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number +  ((ExpressionAtomic*)this->rhs)->number); break;
                case op_sub:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number -  ((ExpressionAtomic*)this->rhs)->number); break;
                case op_mul:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number *  ((ExpressionAtomic*)this->rhs)->number); break;
                case op_div:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number /  ((ExpressionAtomic*)this->rhs)->number); break;
                case op_equal:          return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number == ((ExpressionAtomic*)this->rhs)->number); break;
                case op_not_equal:      return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number != ((ExpressionAtomic*)this->rhs)->number); break;
                case op_greater:        return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number >  ((ExpressionAtomic*)this->rhs)->number); break;
                case op_less:           return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number <  ((ExpressionAtomic*)this->rhs)->number); break;
                case op_greater_equal:  return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number >= ((ExpressionAtomic*)this->rhs)->number); break;
                case op_less_equal:     return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number <= ((ExpressionAtomic*)this->rhs)->number); break;
                case op_and:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number && ((ExpressionAtomic*)this->rhs)->number); break;
                case op_or:             return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->number || ((ExpressionAtomic*)this->rhs)->number); break;
            }
        } else if (((ExpressionAtomic*)this->lhs)->type == t_float) {
            switch (this->type) {
                case op_add:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating +  ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_sub:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating -  ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_mul:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating *  ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_div:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating /  ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_equal:          return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating == ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_not_equal:      return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating != ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_greater:        return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating >  ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_less:           return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating <  ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_greater_equal:  return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating >= ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_less_equal:     return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating <= ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_and:            return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating && ((ExpressionAtomic*)this->rhs)->floating); break;
                case op_or:             return new ExpressionAtomic(((ExpressionAtomic*)this->lhs)->floating || ((ExpressionAtomic*)this->rhs)->floating); break;
            }
        } else if (((ExpressionAtomic*)this->lhs)->type == t_string) {
            std::string s1 = ((ExpressionAtomic*)this->lhs)->str;
            std::string s2 = ((ExpressionAtomic*)this->rhs)->str;
            switch (this->type) {
                case op_add: return new ExpressionAtomic(std::string("\"").append(s1).append(s2).append(std::string("\"")), false); break;
                default: ast->push_err("Unknown binary operation applied to strings."); break;
            }
        }
    }
    return this;
}

llvm::Value* BinaryExpression::codegen(AST* ast) {
    llvm::Value *L = this->lhs->codegen(ast);
    llvm::Value *R = this->rhs->codegen(ast);
    if (!L || !R)
        return nullptr;
    
    if (L->getType() == llvm::Type::getDoubleTy(*(ast->TheContext))) {
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
    } else if (L->getType() == llvm::Type::getInt64Ty(*(ast->TheContext))) {
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
    } else if (L->getType() == llvm::Type::getInt1Ty(*(ast->TheContext))) {
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

UnaryExpression::UnaryExpression(Expression* expr, std::string op) {
    this->operand = expr;
    this->op = op;
}

void UnaryExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "unary " << this->op << std::endl;
    this->operand->debug(depth + 1);
    
}

Expression* UnaryExpression::fold(AST* ast) {
    this->operand = this->operand->fold(ast);
    if (this->operand->get_type() == t_atomic){
        if (this->op.compare("()")) {
            return this->operand;
        }
        if (((ExpressionAtomic*)this->operand)->type == t_number) {
            if (this->op.compare("-")){
                return new ExpressionAtomic(-((ExpressionAtomic*)this->operand)->number);
            }
        } else if (((ExpressionAtomic*)this->operand)->type == t_float) {
            if (this->op.compare("-")){
                return new ExpressionAtomic(-((ExpressionAtomic*)this->operand)->floating);
            }
        } else if (((ExpressionAtomic*)this->operand)->type == t_bool) {
            if (this->op.compare("!")){
                return new ExpressionAtomic(!((ExpressionAtomic*)this->operand)->boolean);
            }
        }
    }
    return (Expression*)this;
}

llvm::Value* UnaryExpression::codegen(AST* ast) {
    return nullptr;
}

AssignmentExpression::AssignmentExpression(Expression* identifier, Expression* value) {
    this->identifier = identifier;
    this->value = value;
}

void AssignmentExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << ((ExpressionAtomic*)this->identifier)->str << " = " << std::endl;
    this->value->debug(depth + 1);
}

Expression* AssignmentExpression::fold(AST* ast) {
    this->value = this->value->fold(ast);
    ast->scope->update_value(ast, ((ExpressionAtomic*)this->identifier)->str, new ScopeValue(this->value, true));
    return (Expression*)this;
}

llvm::Value* AssignmentExpression::codegen(AST* ast) {
    return nullptr;
}

BreakExpression::BreakExpression() {

}

void BreakExpression::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "BREAK" << std::endl;
}

Expression* BreakExpression::fold(AST* ast) {
    return (Expression*)this;
}

llvm::Value* BreakExpression::codegen(AST* ast) {
    return nullptr;
}
