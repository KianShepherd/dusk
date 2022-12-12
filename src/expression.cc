#include "expression.hh"
#include "ast.hh"
#include "struct.hh"

std::string type_string(AST* ast, AtomType type) {
    switch (type) {
        case t_number: return std::string("int");
        case t_long: return std::string("long");
        case t_float: return std::string("float");
        case t_char: return std::string("char");
        case t_string: return std::string("string");
        case t_identifier: {
            // TODO
            return std::string("string todo");
        }
        case t_bool: return std::string("bool");
        case t_function_call: {
            // TODO
            return std::string("call todo");
        }
        case t_bool_arr: return std::string("bool*");
        case t_number_arr: return std::string("int*");
        case t_float_arr: return std::string("float*");
        case t_string_arr: return std::string("string*");
        case t_get_struct: {
            // TODO
            return std::string("struct todo");
        }
        default: return std::string("oh no");
    }
}

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
    this->length = values.size();
    this->int_vals = values;
}

ExpressionAtomic::ExpressionAtomic(AtomType type, int length, std::vector<double> values) {
    this->type = type;
    this->length = values.size();
    this->float_vals = values;
}

ExpressionAtomic::ExpressionAtomic(int length, std::vector<std::string> values) {
    this->type = t_string_arr;
    this->string_vals = values;
    this->length = values.size();
}

ExpressionAtomic::ExpressionAtomic(Expression* base, Expression* operand) {
    this->base = base;
    this->operand = operand;
    this->type = t_dot_exp;
    this->dot_type = (((ExpressionAtomic*)operand)->type == t_identifier)?0:1;
}

ExpressionAtomic::ExpressionAtomic(Expression* base, Struct* s, int index, std::string field_name) {
    this->base = base;
    this->struct_t = s;
    this->number = index;
    this->type = t_get_struct;
    this->str = field_name;
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
        case t_function_call: {
            std::cout << std::string(depth * 4, ' ') << "call " << this->str << " : ";
            std::cout << this->args.size() << " args" << std::endl;
            for (size_t i = 0; i < this->args.size(); i++)
                this->args[i]->debug(depth + 1);
            break;
        }
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
        case t_dot_exp: {
            std::cout << std::string(depth * 4, ' ') << "method chain" << std::endl;
            this->base->debug(depth + 1);
            std::cout << std::string(depth * 4, ' ') << ". ";;
            if (this->dot_type != 0) {
                std::cout << "call";
            } else {
                std::cout << "get";
            }
            std::cout << std::endl;
            this->operand->debug(depth + 1);
            break;
        }
        case t_get_struct: {
            std::cout << std::string(depth * 4, ' ') << "Get " << this->struct_t->name << "." << this->str << " : " << this->number << " from" << std::endl;
            this->base->debug(depth + 1);
            break;
        }
        default: std::cout << "Unknown atomic type" << std::endl; break;
    }
}

std::string ExpressionAtomic::get_atomic_type_str(AST* ast) {
    AtomType value = ast->scope->get_value(this->str);
    if (this->type == t_identifier) {
        if (value == t_null)
            ast->push_err("Attempted to lookup unknown identifier");
        if (this->index) {
            if (value == t_string_arr) {
                value = t_string;
            } else if (value == t_number_arr) {
                value = t_number;
            } else if (value == t_float_arr) {
                value = t_float;
            }
        }
        if (ast->scope->get_value(this->str) == t_struct)
            return ast->scope->get_value_struct(this->str);
        return type_string(ast, value);
    } else if (this->type == t_function_call) {
        if (ast->get_struct(this->str) != nullptr)
            return this->str;
        auto func = ast->func_map[this->str];
        if (func->type == t_struct)
            return func->struct_name;
        return type_string(ast, func->type);
    } else if (this->type == t_get_struct) {
        value = this->struct_t->struct_var_type_map[this->str];
        if (value == t_struct) {
            return this->struct_t->struct_var_map[this->str];
        } else {
            return type_string(ast, value);
        }
    }
    return type_string(ast, this->type);
}

std::string ExpressionAtomic::type_str(AST* ast) {
    switch (this->type) {
        case t_identifier: {
            if (ast->scope->get_value(this->str) == t_struct)
                return ast->scope->get_value_struct(this->str);
            auto type = type_string(ast, ast->scope->get_value(this->str));
            if (this->index) {
                if (type.compare("string*") == 0) {
                    type = std::string("string");
                } else if (type.compare("int*") == 0) {
                    type = std::string("int");
                } else if (type.compare("float*") == 0) {
                    type = std::string("float");
                }
            }
            return type;
        }
        case t_function_call: {
            if (ast->get_struct(this->str) != nullptr)
                return this->str;
            auto func = ast->func_map[this->str];
            if (func->type == t_struct)
                return func->struct_name;
            return type_string(ast, func->type);
        }
        case t_get_struct: {
            if (this->struct_t->struct_var_type_map[this->str] == t_struct)
                return this->struct_t->struct_var_map[this->str];
            return type_string(ast, this->struct_t->struct_var_type_map[this->str]);
        }
        default: return type_string(ast, this->type);
    }
}

AtomType ExpressionAtomic::get_atomic_type(AST* ast) {
    if (this->type == t_identifier) {
        AtomType value = ast->scope->get_value(this->str);
        if (value == t_null)
            ast->push_err("Attempted to lookup unknown identifier.");
        if (this->index) {
            if (value == t_string_arr) {
                value = t_string;
            } else if (value == t_number_arr) {
                value = t_number;
            } else if (value == t_float_arr) {
                value = t_float;
            }
        }
        return value;
    } else if (this->type == t_function_call) {
        for (auto &func : ast->func_definitions) {
            if (this->str.compare(std::get<0>(func)) == 0)
                return std::get<1>(func);
        }
    } else if (this->type == t_get_struct) {
        return this->struct_t->struct_var_type_map[this->str];
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

void ExpressionAtomic::clean(AST* ast) {
    if (this->type == t_function_call) {
        for (auto& e : this->args) {
            e->clean(ast);
        }
        if (this->str.compare("println") != 0 && this->str.compare("print") != 0) {
            if (ast->func_map[this->str]) {
                ast->to_check.push_back(ast->func_map[this->str]);
            }
        }
    } else if (this->type == t_get_struct) {
        this->base->clean(ast);
    }
}

Expression* ExpressionAtomic::fold(AST* ast) {
    if (this->folded)
        return this;
    this->folded = true;
    if (this->type == t_dot_exp) {
        this->base = this->base->fold(ast);
        if (this->dot_type == 0) {
            this->operand = this->operand->fold(ast);
            if ((this->base)->get_atomic_type_keep_identifier(ast) == t_identifier) {
                std::string struct_name = ast->scope->get_value_struct(((ExpressionAtomic*)this->base)->str);
                Struct* strct = ast->get_struct(struct_name);
                return (new ExpressionAtomic(this->base, strct, strct->gen_field_map[((ExpressionAtomic*)this->operand)->str], ((ExpressionAtomic*)this->operand)->str))->fold(ast);
            } else if (((ExpressionAtomic*)this->base)->type == t_function_call) {
                Function* func = ast->func_map[((ExpressionAtomic*)this->base)->str];
                if (func->type != t_struct) {
                    ast->push_err(std::string("Attempted to call . method off non struct object."));
                    return (Expression*)this;
                }
                Struct* strct = ast->get_struct(func->struct_name);
                return (new ExpressionAtomic(this->base, strct, strct->gen_field_map[((ExpressionAtomic*)this->operand)->str], ((ExpressionAtomic*)this->operand)->str))->fold(ast);
            } else if (((ExpressionAtomic*)this->base)->type == t_get_struct) {
                Struct* strct = ((ExpressionAtomic*)this->base)->struct_t;
                if (strct->struct_var_map.find(((ExpressionAtomic*)this->base)->str) == strct->struct_var_map.end()) {
                    ast->push_err(std::string("Attempted to call . method off non struct object."));
                }
                strct = ast->get_struct(strct->struct_var_map[((ExpressionAtomic*)this->base)->str]);
                std::string struct_name = strct->name;
                return (new ExpressionAtomic(this->base, strct, strct->gen_field_map[((ExpressionAtomic*)this->operand)->str], ((ExpressionAtomic*)this->operand)->str))->fold(ast);
            }
            return (Expression*)this;
        } else {
            if ((this->base)->get_atomic_type_keep_identifier(ast) == t_identifier) {
                std::string struct_name = ast->scope->get_value_struct(((ExpressionAtomic*)this->base)->str);
                struct_name.append(((ExpressionAtomic*)this->operand)->str);
                ((ExpressionAtomic*)this->operand)->args.insert(((ExpressionAtomic*)this->operand)->args.begin(), this->base);
                for (auto& arg : ((ExpressionAtomic*)this->operand)->args) {
                    auto a = arg->fold(ast);
                    struct_name.append(a->get_atomic_type_str(ast));
                }
                ((ExpressionAtomic*)this->operand)->str = struct_name;
                return this->operand->fold(ast);
            } else if (((ExpressionAtomic*)this->base)->type == t_function_call) {
                Function* func = ast->func_map[((ExpressionAtomic*)this->base)->str];
                if (func->type != t_struct) {
                    ast->push_err(std::string("Attempted to call . method off non struct object."));
                    return (Expression*)this;
                }
                std::string struct_name = func->struct_name;
                struct_name.append(((ExpressionAtomic*)this->operand)->str);
                ((ExpressionAtomic*)this->operand)->args.insert(((ExpressionAtomic*)this->operand)->args.begin(), this->base);
                for (auto& arg : ((ExpressionAtomic*)this->operand)->args) {
                    auto a = arg->fold(ast);
                    struct_name.append(a->get_atomic_type_str(ast));
                }
                ((ExpressionAtomic*)this->operand)->str = struct_name;
                return this->operand->fold(ast);
            } else if (((ExpressionAtomic*)this->base)->type == t_get_struct) {
                Struct* strct = ((ExpressionAtomic*)this->base)->struct_t;
                if (strct->struct_var_map.find(((ExpressionAtomic*)this->base)->str) == strct->struct_var_map.end()) {
                    ast->push_err(std::string("Attempted to call . method off non struct object."));
                }
                strct = ast->get_struct(strct->struct_var_map[((ExpressionAtomic*)this->base)->str]);
                std::string struct_name = strct->name;
                struct_name.append(((ExpressionAtomic*)this->operand)->str);
            
                ((ExpressionAtomic*)this->operand)->args.insert(((ExpressionAtomic*)this->operand)->args.begin(), this->base);
                for (auto& arg : ((ExpressionAtomic*)this->operand)->args) {
                    auto a = arg->fold(ast);
                    struct_name.append(a->get_atomic_type_str(ast));
                }
                ((ExpressionAtomic*)this->operand)->str = struct_name;
                return this->operand->fold(ast);
            }
        }
    } else if (this->type == t_function_call) {
        for (int i = 0; i < this->args.size(); i++) {
            auto arg = this->args[i];
            if (this->str.compare("print") == 0 || this->str.compare("println") == 0) {
                arg = arg->fold(ast);
                if (arg->get_atomic_type_keep_identifier(ast) == t_identifier && arg->get_atomic_type(ast) == t_struct) {
                    std::string f_name = ast->scope->get_value_struct(((ExpressionAtomic*)arg)->str);
                    f_name.append("__str__");
                    ExpressionAtomic* f = new ExpressionAtomic(f_name, std::vector<Expression*>({arg}));
                    arg = f;
                } else if (((ExpressionAtomic*)arg)->type == t_function_call) {
                    Function* func = ast->func_map[((ExpressionAtomic*)arg)->str];
                    if (func->type != t_struct) {
                        this->args[i] = arg;
                        continue;
                    }
                    std::string f_name = func->struct_name;

                    f_name.append("__str__");
                    ExpressionAtomic* f = new ExpressionAtomic(f_name, std::vector<Expression*>({arg}));
                    arg = f;
                } else if (((ExpressionAtomic*)arg)->type == t_get_struct) {
                    Struct* strct = ((ExpressionAtomic*)arg)->struct_t;
                    if (strct->struct_var_map.find(((ExpressionAtomic*)((ExpressionAtomic*)arg)->base)->str) == strct->struct_var_map.end()) {
                        this->args[i] = arg;
                        continue;
                    }
                    strct = ast->get_struct(strct->struct_var_map[((ExpressionAtomic*)this->base)->str]);
                    std::string f_name = strct->name;
                    f_name.append("__str__");
                    ExpressionAtomic* f = new ExpressionAtomic(f_name, std::vector<Expression*>({arg}));
                    arg = f;
                }
                arg = arg->fold(ast);
            } else {
                if (
                    arg->get_atomic_type_keep_identifier(ast) == t_identifier
                    && ast->scope->get_value(((ExpressionAtomic*)arg)->str) == t_struct
                    && (ast->get_struct(this->str) || (ast->func_map[this->str] && ast->func_map[this->str]->statements != nullptr))
                    && this->str.find("__del__") == std::string::npos
                    && this->str.find("__DECREF__") == std::string::npos
                    && this->str.find("__INCREF__") == std::string::npos
                    ) {
                    auto s_name = ast->scope->get_value_struct(((ExpressionAtomic*)arg)->str);
                    if (!ast->current_block)
                        std::cout << "No block found in func call fold" << std::endl;
                    ast->current_block->push_back(
                        new ExpressionStatement(
                            new ExpressionAtomic(std::string(s_name).append("__INCREF__").append(std::string(s_name)), std::vector<Expression*>({ new ExpressionAtomic(std::string(((ExpressionAtomic*)arg)->str), true) }))
                        )
                    );
                } else {
                    arg = arg->fold(ast);
                }
            }
            this->args[i] = arg;
            
        }
        if (ast->get_struct(this->str) != nullptr) {
            std::string s = std::string("");
            for (auto& arg: this->args) {
                s.append(arg->type_str(ast));
            }
            this->str = this->str.append(std::to_string(this->args.size())).append(s);
        }
    }
    return (Expression*)this;
}

Expression* ExpressionAtomic::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    switch (this->type) {
        case t_number: return new ExpressionAtomic((long)this->number);
        case t_long: return new ExpressionAtomic((long long)this->number);
        case t_float: return new ExpressionAtomic((double)this->floating);
        case t_char: return new ExpressionAtomic((char)this->character);
        case t_string: return new ExpressionAtomic(std::string("\"").append(std::string(this->str)).append(std::string("\"")), false);
        case t_null: return new ExpressionAtomic();;
        case t_bool: return new ExpressionAtomic((bool)this->boolean);
        case t_number_arr:return new ExpressionAtomic(this->type, this->length, std::vector<long long>(this->int_vals));
        case t_float_arr: return new ExpressionAtomic(this->type, this->length, std::vector<double>(this->float_vals));
        case t_string_arr: return new ExpressionAtomic(this->type, this->length, std::vector<long long>(this->int_vals));
        case t_bool_arr: return new ExpressionAtomic(this->length, std::vector<std::string>(this->string_vals));
        case t_identifier: return new ExpressionAtomic(std::string(this->str), true);
        case t_dot_exp: return new ExpressionAtomic(this->base->monomorph(new_name, new_type, old_name, old_type), this->operand->monomorph(new_name, new_type, old_name, old_type));
        case t_function_call: {
            auto new_args = std::vector<Expression*>();
            for (auto& a : this->args) {
                new_args.push_back(a->monomorph(new_name, new_type, old_name, old_type));
            }
            auto func_name = std::string(this->str);
            if (func_name.find(std::string("<").append(old_type).append(std::string(">"))) != std::string::npos)
                func_name.replace(func_name.find(std::string("<").append(old_type).append(std::string(">"))), old_type.length() + 2, std::string("<").append(new_type).append(">"));
            return new ExpressionAtomic(func_name, new_args);
        }
        case t_get_struct: {
            Struct* strct = this->struct_t;
            if (strct->name.compare(old_name) == 0) {
                strct = this->struct_t->ast->get_struct(new_name);
            } else if (strct->name.compare(old_type) == 0) {
                strct = this->struct_t->ast->get_struct(new_type);
            }
            return new ExpressionAtomic(this->base->monomorph(new_name, new_type, old_name, old_type), strct, this->number, std::string(this->str));
        }
        default: std::cout << "Unknown atomic type" << std::endl; return nullptr;
    }
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
            llvm::Value* V = std::get<0>(ast->NamedValues->get_value(this->str));
            if (!V)
                return ast->LogErrorV("Unknown variable name");
            if (!this->index) {
                return ast->Builder->CreateLoad(std::get<1>(ast->NamedValues->get_value(this->str)), V, this->str.c_str());
            } else {
                llvm::Type* atom_type = nullptr;
                AtomType vec_type = std::get<2>(ast->NamedValues->get_value(this->str));
                if (vec_type == t_float_arr) {
                    atom_type = llvm::Type::getDoubleTy(*(ast->TheContext));
                } else if (vec_type == t_number_arr) {
                    atom_type = llvm::Type::getInt64Ty(*(ast->TheContext));
                } else if (vec_type == t_bool_arr) {
                    atom_type = llvm::Type::getInt1Ty(*(ast->TheContext));
                } else if (vec_type == t_string_arr) {
                    atom_type = llvm::Type::getInt8PtrTy(*(ast->TheContext));
                } else if (vec_type == t_string) {
                    atom_type = llvm::Type::getInt8Ty(*(ast->TheContext));
                }
                auto index_val = this->index->codegen(ast, t_number);
                auto index_vec = std::vector<llvm::Value*>();
                index_vec.push_back(index_val);

                auto Vd = ast->Builder->CreateLoad(std::get<1>(ast->NamedValues->get_value(this->str)), V, this->str.c_str());

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
                if (this->str.compare("print") != 0 && this->str.compare("println") != 0) {
                    ArgsV.push_back(this->args[i]->codegen(ast, arg_type[i]));
                } else {
                    ArgsV.push_back(this->args[i]->codegen(ast, t_null));
                }
                if (!ArgsV.back())
                    return nullptr;
            }

            if (CalleeF->getReturnType()->isVoidTy()) {
                return ast->Builder->CreateCall(CalleeF, ArgsV, llvm::Twine::createNull());
            } else {
                return ast->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
            }
        }
        case t_bool_arr: {
            auto atom_type = llvm::Type::getInt1Ty(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("ba"), ast->Builder->GetInsertBlock());
            for (int i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, (bool)this->int_vals[i],false)), ptr);
            }
            return array;
        }
        case t_number_arr: {
            auto atom_type = llvm::Type::getInt64Ty(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*(ast->TheContext)), 1), llvm::Twine("na"), ast->Builder->GetInsertBlock());
            for (int i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_64(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, (long)this->int_vals[i],true)), ptr);
            }
            return array;
        }
        case t_float_arr: {
            auto atom_type = llvm::Type::getDoubleTy(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("fa"), ast->Builder->GetInsertBlock());
            for (int i = 0; i < this->length; i++) {
                auto ptr = ast->Builder->CreateConstGEP1_32(atom_type, array, i);
                ast->Builder->CreateStore(llvm::ConstantFP::get(*(ast->TheContext), llvm::APFloat((double)this->float_vals[i])), ptr);
            }
            return array;
        }
        case t_string_arr: {
            auto atom_type = llvm::Type::getInt8PtrTy(*(ast->TheContext));
            auto type = llvm::ArrayType::get(atom_type, this->length);

            auto array = new llvm::AllocaInst(type, 0, llvm::ConstantInt::get(llvm::Type::getInt32Ty(*(ast->TheContext)), 1), llvm::Twine("sa"), ast->Builder->GetInsertBlock());
            for (int i = 0; i < this->length; i++) {
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
        case t_get_struct: {
            auto real_type = this->struct_t->struct_type;
            auto real_name = this->struct_t->name;
            real_name.append(this->str);
            auto val_ptr = ast->Builder->CreateConstGEP2_32(real_type, this->base->codegen(ast, t_null), 0, this->number,  std::string("struct_field_ptr"));
            // If this is used as an lvalue (e.g. something to write to) we return the adress to write into, rather than the value stored there
            if (type == t_lvalue)
                return val_ptr;
            return ast->Builder->CreateLoad(this->struct_t->field_type_map[this->str], val_ptr, real_name);
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
        case op_modulo: std::cout << "%"; break;
        case op_increment: break;
    }
    std::cout << std::endl;
    this->lhs->debug(depth + 1);
    this->rhs->debug(depth + 1);
    
}

void BinaryExpression::clean(AST* ast) {
    this->lhs->clean(ast);
    this->rhs->clean(ast);
}

Expression* BinaryExpression::fold(AST* ast) {
    this->lhs = this->lhs->fold(ast);
    this->rhs = this->rhs->fold(ast);
    if (this->lhs->get_atomic_type(ast) == t_struct || this->rhs->get_atomic_type(ast) == t_struct) {
        std::string f_name = std::string("");
        if (this->lhs->get_atomic_type_keep_identifier(ast) == t_identifier && this->lhs->get_atomic_type(ast) == t_struct) {
            f_name.append(ast->scope->get_value_struct(((ExpressionAtomic*)this->lhs)->str));
        } else if (((ExpressionAtomic*)this->lhs)->type == t_function_call) {
            Function* func = ast->func_map[((ExpressionAtomic*)this->lhs)->str];
            if (func->type != t_struct) {
                f_name.append(type_string(ast, func->type));
            } else {
                f_name.append(func->struct_name);
            }
        } else if (((ExpressionAtomic*)this->lhs)->type == t_get_struct) {
            Struct* strct = ((ExpressionAtomic*)this->lhs)->struct_t;
            if (strct->struct_var_map.find(((ExpressionAtomic*)this->lhs)->str) == strct->struct_var_map.end()) {
                return (Expression*)this;
            }
            strct = ast->get_struct(strct->struct_var_map[((ExpressionAtomic*)this->lhs)->str]);
            f_name.append(strct->name);
        } else {
            f_name.append(type_string(ast, this->lhs->get_atomic_type(ast)));
        }
        if (this->rhs->get_atomic_type_keep_identifier(ast) == t_identifier && this->rhs->get_atomic_type(ast) == t_struct) {
            f_name.append(ast->scope->get_value_struct(((ExpressionAtomic*)this->rhs)->str));
        } else if (((ExpressionAtomic*)this->rhs)->type == t_function_call) {
            Function* func = ast->func_map[((ExpressionAtomic*)this->rhs)->str];
            if (func->type != t_struct) {
                f_name.append(type_string(ast, func->type));
            } else {
                f_name.append(func->struct_name);
            }
        } else if (((ExpressionAtomic*)this->rhs)->type == t_get_struct) {
            Struct* strct = ((ExpressionAtomic*)this->rhs)->struct_t;
            if (strct->struct_var_map.find(((ExpressionAtomic*)this->rhs)->str) == strct->struct_var_map.end()) {
                return (Expression*)this;
            }
            strct = ast->get_struct(strct->struct_var_map[((ExpressionAtomic*)this->rhs)->str]);
            f_name.append(strct->name);
        } else {
            f_name.append(type_string(ast, this->rhs->get_atomic_type(ast)));
        }
        
        switch (this->type) {
            case op_add:           f_name.append("__add__"); break;
            case op_sub:           f_name.append("__sub__"); break;
            case op_mul:           f_name.append("__mul__"); break;
            case op_div:           f_name.append("__div__"); break;
            case op_equal:         f_name.append("__eq__"); break;
            case op_not_equal:     f_name.append("__neq__"); break;
            case op_greater:       f_name.append("__gt__"); break;
            case op_less:          f_name.append("__lt__"); break;
            case op_greater_equal: f_name.append("__ge__"); break;
            case op_less_equal:    f_name.append("__le__"); break;
            case op_and:           f_name.append("__and__"); break;
            case op_or:            f_name.append("__or__"); break;
            case op_modulo:        f_name.append("__mod__"); break;
            case op_increment:     f_name.append("__increment__"); break;
        }
        return (new ExpressionAtomic(f_name, std::vector<Expression*>({this->lhs, this->rhs})))->fold(ast);
    }
    return (Expression*)this;
}

Expression* BinaryExpression::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return new BinaryExpression(this->lhs->monomorph(new_name, new_type, old_name, old_type), this->rhs->monomorph(new_name, new_type, old_name, old_type), this->type);
}

std::string BinaryExpression::get_atomic_type_str(AST* ast) {
    return this->lhs->get_atomic_type_str(ast);
}

std::string BinaryExpression::type_str(AST* ast) {
    if (this->lhs->get_atomic_type(ast) != this->rhs->get_atomic_type(ast)) {
        ast->push_err("Both operands of a binary operator must be of same type");
    }
    return this->lhs->type_str(ast);
}

llvm::Value* BinaryExpression::codegen(AST* ast, AtomType type) {
    if (this->lhs->get_atomic_type_keep_identifier(ast) == t_identifier) {
        if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->lhs)->str))->isIntegerTy(8)) {
            type = t_char;
        } else if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->lhs)->str))->isIntegerTy(64)) {
            type = t_number;
        } else if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->lhs)->str))->isIntegerTy(128)) {
            type = t_long;
        }
    } else if (this->rhs->get_atomic_type_keep_identifier(ast) == t_identifier) {
        if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->rhs)->str))->isIntegerTy(8)) {
            type = t_char;
        } else if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->rhs)->str))->isIntegerTy(64)) {
            type = t_number;
        } else if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->rhs)->str))->isIntegerTy(128)) {
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
            case op_modulo:         return ast->Builder->CreateFRem(L, R);
            case op_increment:      return nullptr;
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
            case op_modulo:         return ast->Builder->CreateSRem(L, R);
            case op_increment:      return nullptr;
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
            case op_modulo:         return ast->Builder->CreateSRem(L, R);
            case op_increment:      return nullptr;
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
            case op_modulo:         return ast->Builder->CreateSRem(L, R);
            case op_increment:      return nullptr;
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
            case op_modulo:         return nullptr;
            case op_increment:      return nullptr;
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

void UnaryExpression::clean(AST* ast) {
    this->operand->clean(ast);
}

Expression* UnaryExpression::fold(AST* ast) {
    this->operand = this->operand->fold(ast);
    if (this->op.compare("++") == 0) {
        if (this->operand->get_atomic_type_keep_identifier(ast) != t_identifier) {
            ast->push_err("++ can only be applied to indentifiers.");
            return (Expression*)this;
        }
        
        return ((Expression*)new AssignmentExpression(this->operand, ((Expression*)new BinaryExpression(this->operand, new ExpressionAtomic((long)1), op_add))))->fold(ast);
    }
    if (this->op.compare("()") == 0) {
        return this->operand;
    }
    return (Expression*)this;
}

Expression* UnaryExpression::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return new UnaryExpression(this->operand->monomorph(new_name, new_type, old_name, old_type), std::string(this->op));
}

std::string UnaryExpression::get_atomic_type_str(AST* ast) {
    return this->operand->get_atomic_type_str(ast);
}

std::string UnaryExpression::type_str(AST* ast) {
    return this->operand->type_str(ast);
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
    this->is_methods = 0;
}

AssignmentExpression::AssignmentExpression(Expression* identifier, Expression* value, Expression* index) {
    this->identifier = identifier;
    this->value = value;
    this->index = index;
    this->is_arr = true;
    this->is_methods = 0;
}

AssignmentExpression::AssignmentExpression(Expression* identifier, Expression* value, int is_methods) {
    this->identifier = identifier;
    this->value = value;
    this->index = 0;
    this->is_arr = false;
    this->is_methods = 1;
}

void AssignmentExpression::debug(size_t depth) {
    if (!this->is_methods) {
        std::cout << std::string(depth * 4, ' ') << ((ExpressionAtomic*)this->identifier)->str;
    } else {
        this->identifier->debug(depth+1);
    }
    if (this->index) {
        std::cout << std::string(depth * 4, ' ') << "[";
        this->index->debug(depth);
        std::cout << std::string(depth * 4, ' ') << "]";
    }
    std::cout << std::string(depth * 4, ' ') << " = " << std::endl;
    this->value->debug(depth + 1);
}

void AssignmentExpression::clean(AST* ast) {
    this->value->clean(ast);
}

Expression* AssignmentExpression::fold(AST* ast) {
    this->value = this->value->fold(ast);
    this->identifier = this->identifier->fold(ast);
    if (this->value->get_atomic_type_keep_identifier(ast) == t_identifier && this->value->get_atomic_type(ast) == t_struct) {
        std::string s_name = ast->scope->get_value_struct(((ExpressionAtomic*)this->identifier)->str);
        ast->current_block->push_back(new ExpressionStatement(new ExpressionAtomic(std::string(s_name).append("__INCREF__").append(s_name), std::vector<Expression*>({new ExpressionAtomic(std::string(((ExpressionAtomic*)this->identifier)->str), true)}))));
    }
    if (this->identifier->get_atomic_type(ast) == t_struct) {
        std::string s_name = ast->scope->get_value_struct(((ExpressionAtomic*)this->identifier)->str);
        ast->current_block->push_back(new ExpressionStatement(new ExpressionAtomic(std::string(s_name).append("__DECREF__").append(s_name), std::vector<Expression*>({new ExpressionAtomic(std::string(((ExpressionAtomic*)this->identifier)->str), true)}))));
    }
    if (this->is_methods)
        return (Expression*)this;
    if (this->is_arr) {
        ast->scope->update_value(ast, ((ExpressionAtomic*)this->identifier)->str, new ScopeValue(true, ((ExpressionAtomic*)this->value)->get_atomic_type(ast), std::string("")));
    } else {
        ast->scope->update_value(ast, ((ExpressionAtomic*)this->identifier)->str, new ScopeValue(true, ((ExpressionAtomic*)this->value)->get_atomic_type(ast), std::string("")));
    }
    return (Expression*)this;
}

Expression* AssignmentExpression::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    if (this->is_methods) {
        return new AssignmentExpression(this->identifier->monomorph(new_name, new_type, old_name, old_type), this->value->monomorph(new_name, new_type, old_name, old_type), 1);
    } else if (this->is_arr) {
        return new AssignmentExpression(this->identifier->monomorph(new_name, new_type, old_name, old_type), this->value->monomorph(new_name, new_type, old_name, old_type), this->index->monomorph(new_name, new_type, old_name, old_type));
    } else {
        return new AssignmentExpression(this->identifier->monomorph(new_name, new_type, old_name, old_type), this->value->monomorph(new_name, new_type, old_name, old_type));
    }
}

std::string AssignmentExpression::get_atomic_type_str(AST* ast) {
    return this->value->get_atomic_type_str(ast);
}

std::string AssignmentExpression::type_str(AST* ast) {
    return std::string("");
}

llvm::Value* AssignmentExpression::codegen(AST* ast, AtomType type) {
    if (!this->is_methods) {
        AtomType t = t_null;
        if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->identifier)->str))->isIntegerTy(8)) {
            t = t_char;
        } else if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->identifier)->str))->isIntegerTy(64)) {
            t = t_number;
        } else if (std::get<1>(ast->NamedValues->get_value(((ExpressionAtomic*)this->identifier)->str))->isIntegerTy(128)) {
            t = t_long;
        }
        llvm::Value *Val = this->value->codegen(ast, t);
        if (!Val)
            return nullptr;

        llvm::Value *Variable = std::get<0>(ast->NamedValues->get_value(((ExpressionAtomic*)this->identifier)->str));
        if (!Variable)
            return ast->LogErrorV("Unknown variable name");
        if (!this->is_arr) {
            ast->Builder->CreateStore(Val, Variable);
        } else {
            llvm::Type* atom_type = nullptr;
            AtomType vec_type= std::get<2>(ast->NamedValues->get_value(((ExpressionAtomic*)this->identifier)->str));
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
    } else {
        llvm::Value* Variable = this->identifier->codegen(ast, t_lvalue);
        if (!Variable)
            return nullptr;
        llvm::Value *Val = this->value->codegen(ast, t_null);
        if (!Val)
            return nullptr;
        ast->Builder->CreateStore(Val, Variable);
        return Val;
    }
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

void BreakExpression::clean(AST* ast) {

}

Expression* BreakExpression::fold(AST* ast) {
    return (Expression*)this;
}

Expression* BreakExpression::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
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

std::string BreakExpression::type_str(AST* ast) {
    return std::string("");
}
std::string BreakExpression::get_atomic_type_str(AST* ast) {
    return std::string("");
}
