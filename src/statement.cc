#include "statement.hh"
#include "ast.hh"

ExpressionStatement::ExpressionStatement(Expression* expr) {
    this->expr = expr;
}

void ExpressionStatement::debug(size_t depth) {
    this->expr->debug(depth);
}

void ExpressionStatement::clean(AST* ast) {
    this->expr->clean(ast);
}

void ExpressionStatement::fold(AST* ast, std::vector<Statement*>& block) {
    this->expr = this->expr->fold(ast);
    // TODO: ARC call DECREF on struct if expr returns a struct
    block.push_back(this);
}

Statement* ExpressionStatement::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
}

llvm::Value* ExpressionStatement::codegen(AST* ast) {
    return this->expr->codegen(ast, t_null);
}

StatementBlock::StatementBlock(std::vector<Statement*> statements) {
    this->statements = statements;
    this->returned = false;
}

void StatementBlock::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "Block {" << std::endl;
    for (size_t i = 0; i < this->statements.size(); i++) {
        std::cout << std::string((depth + 1) * 4 - 2, ' ') << "Statement: " << i << std::endl;
        this->statements[i]->debug(depth + 1);
    }
    std::cout << std::string(depth * 4, ' ') << "}" << std::endl;
}

void StatementBlock::clean(AST* ast) {
    for (size_t i = 0; i < this->statements.size(); i++) {
        this->statements[i]->clean(ast);
    }
}

void StatementBlock::fold(AST* ast, std::vector<Statement*>& block) {
    std::vector<Statement*>* new_stats = new std::vector<Statement*>();;
    auto prev_block = ast->current_block;
    auto prev_returned = ast->block_returned;
    ast->current_block = new_stats;
    ast->block_returned = false;
    ast->push_scope();
    for (size_t i = 0; i < this->statements.size(); i++) {
        this->statements[i]->fold(ast, *new_stats);
    }
    this->returned = ast->block_returned;
    if (!ast->block_returned) {
        std::map<std::string, ScopeValue*> m = ast->scope->get_all();
        for(std::map<std::string, ScopeValue*>::iterator it = m.begin(); it != m.end(); ++it) {
            if (it->second->type == t_struct) {
                new_stats->push_back(
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
    ast->pop_scope();
    this->statements = *new_stats;
    ast->current_block = prev_block;
    ast->block_returned = prev_returned;
    block.push_back(this);
}

Statement* StatementBlock::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
}

llvm::Value* StatementBlock::codegen(AST* ast) {
    llvm::Value* last = nullptr;
    ast->NamedValues = ast->NamedValues->new_scope();
    for (long unsigned int i = 0; i < this->statements.size(); i++) {
        last = this->statements[i]->codegen(ast);
    }
    auto frame = ast->NamedValues;
    ast->NamedValues = (frame->prev_frame)?frame->prev_frame:frame;
    delete frame;
    return last;
}

ReturnStatement::ReturnStatement(Expression* expr) {
    this->expr = expr;
}

void ReturnStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ')<< "RETURN";
    if (this->expr) {
        std::cout << std::endl;
        this->expr->debug(depth + 1);
    } else {
        std::cout << " void" << std::endl;
    }
}

void ReturnStatement::clean(AST* ast) {
    if (this->expr)
        this->expr->clean(ast);
}

void ReturnStatement::fold(AST* ast, std::vector<Statement*>& block) {
    ast->block_returned = true;
    if (this->expr) {
        this->expr = this->expr->fold(ast);
        if (this->expr->get_atomic_type_keep_identifier(ast) == t_identifier) {
            if (ast->get_struct(this->expr->type_str(ast))) {
                block.push_back(
                new ExpressionStatement(
                        new ExpressionAtomic(
                                std::string(this->expr->type_str(ast)).append("__INCREF__").append(std::string(this->expr->type_str(ast))),
                                std::vector<Expression*>({ this->expr })
                        )
                    )
                );
            }
        } else if (this->expr->get_atomic_type_keep_identifier(ast) == t_get_struct && ((ExpressionAtomic*)this->expr)->struct_t->struct_var_type_map[((ExpressionAtomic*)this->expr)->str] == t_struct) {
                ExpressionAtomic* exp = (ExpressionAtomic*)this->expr;
                std::string struct_name = exp->struct_t->struct_var_map[exp->str];
                block.push_back(
                    new ExpressionStatement(
                        new ExpressionAtomic(
                                std::string(struct_name).append("__INCREF__").append(std::string(struct_name)),
                                std::vector<Expression*>({ this->expr })
                        )
                    )
                );
        }
    }

    auto cur_scope = ast->scope;
    std::vector<std::string> key;
    std::vector<ScopeValue*> value;
    std::map<std::string, std::string> structs_to_drop;
    std::vector<std::string> struct_idents;
    while (true) {
        std::map<std::string, ScopeValue*> m = cur_scope->get_all();
        for(std::map<std::string, ScopeValue*>::iterator it = m.begin(); it != m.end(); ++it) {
            key.push_back(it->first);
            value.push_back(it->second);
            if (it->second->type == t_struct) {
                block.push_back(
                    new ExpressionStatement(
                        new ExpressionAtomic(
                            std::string(it->second->struct_name).append("__DECREF__").append(std::string(it->second->struct_name)),
                            std::vector<Expression*>({ new ExpressionAtomic(std::string(it->first), true) })
                        )
                    )
                );
            }
        }

        if (cur_scope->prev_frame) {
            cur_scope = cur_scope->prev_frame;
        } else {
            break;
        }
    }
    block.push_back(this);
}

Statement* ReturnStatement::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
}

llvm::Value* ReturnStatement::codegen(AST* ast) {
    if (!this->expr) {
        ast->Builder->CreateRetVoid();
        return nullptr;
    }
    auto retval = this->expr->codegen(ast, t_null);
    auto type = this->expr->get_atomic_type_keep_identifier(ast);
    if (type == t_identifier) {
        auto named_value = ast->NamedValues->get_value(((ExpressionAtomic*) this->expr)->str);
        type = std::get<2>(named_value);
        long length = std::get<3>(named_value);
        if (type == t_float_arr) {
            llvm::Function *CalleeF = ast->TheModule->getFunction("copyd");
            if (!CalleeF)
                return ast->LogErrorV("Unknown function referenced");

            std::vector<llvm::Value*> ArgsV;
            ArgsV.push_back(retval);
            ArgsV.push_back(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, length, true)));
            retval = ast->Builder->CreateCall(CalleeF, ArgsV, "copytmp");
        } else if (type == t_string_arr) {
            llvm::Function *CalleeF = ast->TheModule->getFunction("copysa");
            if (!CalleeF)
                return ast->LogErrorV("Unknown function referenced");

            std::vector<llvm::Value*> ArgsV;
            ArgsV.push_back(retval);
            ArgsV.push_back(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, length, true)));
            retval = ast->Builder->CreateCall(CalleeF, ArgsV, "copytmp");
        } else if (type == t_number_arr) {
            llvm::Function *CalleeF = ast->TheModule->getFunction("copyi");
            if (!CalleeF)
                return ast->LogErrorV("Unknown function referenced");

            std::vector<llvm::Value*> ArgsV;
            ArgsV.push_back(retval);
            ArgsV.push_back(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, length, true)));
            retval = ast->Builder->CreateCall(CalleeF, ArgsV, "copytmp");
        } else if (type == t_string) {
            llvm::Function *CalleeF = ast->TheModule->getFunction("copys");
            if (!CalleeF)
                return ast->LogErrorV("Unknown function referenced");

            std::vector<llvm::Value*> ArgsV;
            ArgsV.push_back(retval);
            ArgsV.push_back(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(64, length, true)));
            retval = ast->Builder->CreateCall(CalleeF, ArgsV, "copytmp");
        }
    }
    ast->Builder->CreateRet(retval);
    return nullptr;
}

AssignmentStatement::AssignmentStatement(Expression* identifier, Expression* value, bool mut, AtomType type, long length) {
    this->identifier = identifier;
    this->value = value;
    this->mut = mut;
    this->type = type;
    this->length = length;
}

AssignmentStatement::AssignmentStatement(Expression* identifier, Expression* value, bool mut, AtomType type, std::string struct_name) {
    this->identifier = identifier;
    this->value = value;
    this->mut = mut;
    this->type = type;
    this->length = -1;
    this->struct_name = struct_name;
}

void AssignmentStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ');
    std::cout << "let " << ((this->mut)?"mutable ":"static ") << ((ExpressionAtomic*)this->identifier)->str << " : ";
    switch (this->type) {
        case t_bool: std::cout << "bool "; break;
        case t_number: std::cout << "int "; break;
        case t_long: std::cout << "long "; break;
        case t_char: std::cout << "char "; break;
        case t_float: std::cout << "float "; break;
        case t_string: std::cout << "string "; break;
        case t_bool_arr: std::cout << "bool* "; break;
        case t_number_arr: std::cout << "int* "; break;
        case t_float_arr: std::cout << "float* "; break;
        case t_string_arr: std::cout << "string* "; break;
        case t_struct: std::cout << this->struct_name << " "; break;
        default: std::cerr << "Unknown type for assignment"; break;
    }
    std::cout << " = " << std::endl;
    if (this->value) {
        this->value->debug(depth + 1);
    } else {
        std::cout << std::string(' ', (depth + 1) * 4) << "len : " << this->length << std::endl;
    }
}

void AssignmentStatement::clean(AST* ast) {
    if (this->value)
        this->value->clean(ast);
}

void AssignmentStatement::fold(AST* ast, std::vector<Statement*>& block) {

    this->debug(0);
    if (this->type == t_struct) {
        ast->get_struct(this->struct_name);
    }
    if (this->value) {
        this->value = this->value->fold(ast);
        if (this->value->get_atomic_type_keep_identifier(ast) == t_identifier && this->value->get_atomic_type(ast) == t_struct) {
            std::string s_name = ast->scope->get_value_struct(((ExpressionAtomic*)this->value)->str);
            ast->current_block->push_back(new ExpressionStatement(new ExpressionAtomic(std::string(s_name).append("__INCREF__").append(s_name), std::vector<Expression*>({new ExpressionAtomic(std::string(((ExpressionAtomic*)this->value)->str), true)}))));
        }
        if (this->value->get_type() == t_get) {
            ast->current_block->push_back(new ExpressionStatement(
                new ExpressionAtomic(
                    std::string(this->struct_name).append("__INCREF__").append(this->struct_name),
                    std::vector<Expression*>({this->value})
                )
            ));
        }
        if (this->type == t_struct) {
            ast->scope->push_value(((ExpressionAtomic*)this->identifier)->str, new ScopeValue(this->mut, this->type, this->struct_name));
        } else {
            ast->scope->push_value(((ExpressionAtomic*)this->identifier)->str, new ScopeValue(this->mut, this->value->get_atomic_type(ast), std::string("")));
        }
    } else {
        if (this->type == t_struct) {
            ast->scope->push_value(((ExpressionAtomic*)this->identifier)->str, new ScopeValue(this->mut, this->type, this->struct_name));
        } else {
            ast->scope->push_value(((ExpressionAtomic*)this->identifier)->str, new ScopeValue(this->mut, this->type, std::string("")));
        }
    }
    block.push_back(this);
}

Statement* AssignmentStatement::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
}

llvm::Value* AssignmentStatement::codegen(AST* ast) {
    llvm::Function *TheFunction = ast->Builder->GetInsertBlock()->getParent();

    // Register all variables and emit their initializer.
    const std::string VarName = ((ExpressionAtomic*)this->identifier)->str;
    Expression* Init;
    if (this->value) {
        Init = this->value;
        if (this->type == t_float_arr || this->type == t_number_arr || this->type == t_bool_arr || this->type == t_string_arr || this->type == t_string)
            this->length = ((ExpressionAtomic*)this->value)->length;
        if (this->type == t_struct) {
            this->length = 1;
        }
    } else {
        if (this->type == t_float_arr) {
            std::vector<double> vals;
            for (int i = 0; i < this->length; i++) {
                vals.push_back(0.0);
            }
            Init = new ExpressionAtomic(this->type, this->length, vals);
        } else if (this->type == t_string_arr) {
            std::vector<std::string> vals;
            for (int i = 0; i < this->length; i++) {
                vals.push_back(std::string(" "));
            }
            Init = new ExpressionAtomic(this->length, vals);
        } else {
            std::vector<long long> vals;
            for (int i = 0; i < this->length; i++) {
                vals.push_back(0);
            }
            Init = new ExpressionAtomic(this->type, this->length, vals);
        }
    }

    // Emit the initializer before adding the variable to scope, this prevents
    // the initializer from referencing the variable itself, and permits stuff
    // like this:
    //  var a = 1 in
    //    var a = a in ...   # refers to outer 'a'.
    llvm::Value* InitVal = Init->codegen(ast, this->type);
    llvm::Type* init_type;
    switch (this->type) {
        case t_bool: init_type = llvm::Type::getInt1Ty(*(ast->TheContext)); break;
        case t_number: init_type = llvm::Type::getInt64Ty(*(ast->TheContext)); break;
        case t_long: init_type = llvm::Type::getInt128Ty(*(ast->TheContext)); break;
        case t_float: init_type = llvm::Type::getDoubleTy(*(ast->TheContext)); break;
        case t_char: init_type = llvm::Type::getInt8Ty(*(ast->TheContext)); break;
        case t_string: init_type = llvm::Type::getInt8PtrTy(*(ast->TheContext)); break;
        case t_struct: init_type = llvm::Type::getInt8PtrTy(*(ast->TheContext)); break;
        case t_bool_arr: init_type = llvm::Type::getInt1PtrTy(*(ast->TheContext)); break;
        case t_number_arr: init_type = llvm::Type::getInt64PtrTy(*(ast->TheContext)); break;
        case t_float_arr: init_type = llvm::Type::getDoublePtrTy(*(ast->TheContext)); break;
        case t_string_arr: init_type = llvm::Type::getInt8PtrTy(*(ast->TheContext))->getPointerTo(); break;
        default: return nullptr; break;
    }
    llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(ast, TheFunction, VarName, init_type);
    ast->Builder->CreateStore(InitVal, Alloca);

    // Remember this binding.
    ast->NamedValues->push_value(VarName, std::make_tuple(Alloca, init_type, this->type, this->length));

    // Return the body computation.
    return InitVal;
}

IfStatement::IfStatement(Expression* condition, Statement* block1, Statement* block2) {
    this->condition = condition;
    this->block1 = block1;
    this->block2 = block2;
}

void IfStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "If" << std::endl;
    this->condition->debug(depth + 1);
    std::cout << std::string(depth * 4, ' ') << "Then" << std::endl;
    this->block1->debug(depth + 1);
    if (this->block2 != nullptr) {
        std::cout << std::string(depth * 4, ' ') << "Else" << std::endl;
        this->block2->debug(depth + 1);
    }
}

void IfStatement::clean(AST* ast) {
    this->condition->clean(ast);
    if (this->block1) {
        this->block1->clean(ast);
    }
    if (this->block2) {
        this->block2->clean(ast);
    }
}

void IfStatement::fold(AST* ast, std::vector<Statement*>& block) {
    this->condition = this->condition->fold(ast);
    std::vector<Statement*> null_block;
    if (this->block1) {
        this->block1->fold(ast, null_block);
    }
    if (this->block2) {
        this->block2->fold(ast, null_block);
    }
    block.push_back(this);
}

Statement* IfStatement::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
}

llvm::Value* IfStatement::codegen(AST* ast) {
    llvm::Value *CondV = this->condition->codegen(ast, t_bool);
    if (!CondV)
        return nullptr;

    llvm::BasicBlock* EntryBB = ast->Builder->GetInsertBlock();
    llvm::Function *TheFunction = ast->Builder->GetInsertBlock()->getParent();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*(ast->TheContext), "then", TheFunction);
    llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*(ast->TheContext), "else");
    llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*(ast->TheContext), "ifcont");

    if (this->block2) {
        ast->Builder->CreateCondBr(CondV, ThenBB, ElseBB);
    } else {
        ast->Builder->CreateCondBr(CondV, ThenBB, MergeBB);
    }

    // Emit then value.
    ast->Builder->SetInsertPoint(ThenBB);

    auto b1 = this->block1->codegen(ast);

    if (b1)
        ast->Builder->CreateBr(MergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = ast->Builder->GetInsertBlock();
    llvm::Value* b2 = nullptr;
    if (this->block2) {
        // Emit else block.
        TheFunction->getBasicBlockList().push_back(ElseBB);
        ast->Builder->SetInsertPoint(ElseBB);

        b2 = this->block2->codegen(ast);
    }

    if (b2)
        ast->Builder->CreateBr(MergeBB);
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = ast->Builder->GetInsertBlock();

    unsigned int incoming = 0;
    if (b1)
        incoming++;
    if (b2)
        incoming++;
    if (b1 || b2 || !this->block2) {
        // Emit merge block.
        TheFunction->getBasicBlockList().push_back(MergeBB);
        ast->Builder->SetInsertPoint(MergeBB);

        llvm::PHINode *PN = ast->Builder->CreatePHI(llvm::Type::getInt1Ty(*(ast->TheContext)), incoming, "iftmp");

        if (b1)
            PN->addIncoming(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, 0, false)), ThenBB);
        if (this->block2) {
            PN->addIncoming(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, 1, false)), ElseBB);
        } else {
            PN->addIncoming(llvm::ConstantInt::get(*(ast->TheContext), llvm::APInt(1, 1, false)), EntryBB);
        }
        return PN;
    }
    return nullptr;
}

WhileStatement::WhileStatement(Expression* cond, Statement* block) {
    this->condition = cond;
    this->block = block;
}

void WhileStatement::debug(size_t depth) {
    std::cout << std::string(depth * 4, ' ') << "While" << std::endl;
    this->condition->debug(depth + 1);
    std::cout << std::string(depth * 4, ' ') << "Do" << std::endl;
    this->block->debug(depth + 1);
}

void WhileStatement::clean(AST* ast) {
    this->condition->clean(ast);
    this->block->clean(ast);
}

void WhileStatement::fold(AST* ast, std::vector<Statement*>& _block) {
    std::vector<Statement*> null_block;
    this->condition = this->condition->fold(ast);
    this->block->fold(ast, null_block);
    _block.push_back(this);
}

Statement* WhileStatement::monomorph(std::string new_name, std::string new_type, std::string old_name, std::string old_type) {
    return this;
}

llvm::Value* WhileStatement::codegen(AST* ast) {
    llvm::Value *StartVal = this->condition->codegen(ast, t_null);
    if (!StartVal)
        return nullptr;

    // Make the new basic block for the loop header, inserting after current
    // block.
    llvm::Function *TheFunction = ast->Builder->GetInsertBlock()->getParent();
    llvm::BasicBlock *PreheaderBB = ast->Builder->GetInsertBlock();
    llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(*(ast->TheContext), "loop", TheFunction);

    // Create the "after loop" block and insert it.
    llvm::BasicBlock *AfterBB = llvm::BasicBlock::Create(*(ast->TheContext), "afterloop", TheFunction);

    // Insert an explicit fall through from the current block to the LoopBB.
    //ast->Builder->CreateBr(LoopBB);
    ast->Builder->CreateCondBr(StartVal, LoopBB, AfterBB);

    // Start insertion in LoopBB.
    ast->Builder->SetInsertPoint(LoopBB);

    // Start the PHI node with an entry for Start.
    llvm::PHINode *Variable = ast->Builder->CreatePHI(llvm::Type::getInt1Ty(*(ast->TheContext)), 2, "looptmp");
    Variable->addIncoming(StartVal, PreheaderBB);

    // Within the loop, the variable is defined equal to the PHI node.  If it
    // shadows an existing variable, we have to restore it, so save it now.
    //Value *OldVal = NamedValues[VarName];
    //NamedValues[VarName] = Variable;

    // Emit the body of the loop.  This, like any other expr, can change the
    // current BB.  Note that we ignore the value computed by the body, but don't
    // allow an error.
    this->block->codegen(ast);

    // Compute the end condition.
    llvm::Value *EndCond = this->condition->codegen(ast, t_null);
    if (!EndCond)
        return nullptr;


    // Insert the conditional branch into the end of LoopEndBB.
    ast->Builder->CreateCondBr(EndCond, LoopBB, AfterBB);

    // Any new code will be inserted in AfterBB.
    ast->Builder->SetInsertPoint(AfterBB);

    // Add a new entry to the PHI node for the backedge.
    Variable->addIncoming(EndCond, LoopBB);

    return llvm::Constant::getNullValue(llvm::Type::getInt64Ty(*(ast->TheContext)));
}
