#include "visitor.h"

int PrintVisitor::visit(Program* p) {
    if (p->cl)
        p->cl->accept(this);
    if (p->vl)
        p->vl->accept(this);
    if (p->fl)
        p->fl->accept(this);
    return 0;
}

int PrintVisitor::visit(BinaryExp* exp) {
    cout << '(';
    exp->left->accept(this);
    cout << ' ' << Exp::binopToString(exp->op) << ' ';
    exp->right->accept(this);
    cout << ')';
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if (exp->value)
        cout << "true";
    else
        cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

int PrintVisitor::visit(AssignStatement* stm) {
    cout << stm->id << " = ";
    stm->rhs->accept(this);
    return 0;
}

int PrintVisitor::visit(PrintStatement* stm) {
    if (stm->type == "print") {
        cout << "print(";
        stm->exp->accept(this);
        cout << ')';
    } else {
        cout << "println(";
        stm->exp->accept(this);
        cout << ')';
    }
    cout.flush();
    return 0;
}

int PrintVisitor::visit(IfStatement* stm) {
    cout << "if( ";
    stm->condition->accept(this);
    cout << ") {" << endl;
    stm->then->accept(this);
    cout << " }" << endl;
    if (stm->els) {
        cout << "else {" << endl;
        stm->els->accept(this);
        cout << "\n} " << endl;
    }
    return 0;
}

int PrintVisitor::visit(ForStatement* stm) {
    cout << "for( ";
    cout << stm->begin->id;
    cout << " in ";
    stm->begin->rhs->accept(this);
    cout << " .. ";
    stm->end->accept(this);
    cout << " ) {";
    stm->block->accept(this);
    cout << "}";
    return 0;
}

int PrintVisitor::imprimir(Program* program) {
    program->accept(this);
    return 0;
}

int PrintVisitor::visit(VarDec* stm) {
    stm->is_mut ? cout << "var " : cout << "val ";
    cout << stm->var;
    if (!stm->type.empty()) {
        cout << ": " << stm->type;
    }
    if (stm->stm != nullptr) {
        cout << " = ";
        stm->stm->rhs->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(VarDecList* stm) {
    for (const auto& i : stm->vardecs) {
        i->accept(this);
        cout << '\n';
    }
    return 0;
}

int PrintVisitor::visit(StmtList* stm) {
    for (const auto& i : stm->stm_list) {
        i->accept(this);
        cout << '\n';
    }
    return 0;
}

int PrintVisitor::visit(Block* b) {
    b->vardecl->accept(this);
    b->stmdecl->accept(this);
    return 0;
}

int PrintVisitor::visit(WhileStatement* stm) {
    cout << "while (";
    stm->condition->accept(this);
    cout << " ) {";
    stm->block->accept(this);
    cout << "}";
    return 0;
}

int PrintVisitor::visit(ReturnStatement* stm) {
    cout << "return";
    if (stm->ret != nullptr) {
        cout << ' ';
        stm->ret->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(UnaryExp* exp) {
    exp->op == UnaryOp::NOT_OP ? cout << "!" : cout << "-";
    exp->exp->accept(this);
    return 0;
}

int PrintVisitor::visit(ParamList* pl) {
    bool first = true;
    for (Param* param : pl->param_list) {
        if (!first) {
            std::cout << ", ";
        }
        param->accept(this);
        first = false;
    }
    return 0;
}

int PrintVisitor::visit(Param* p) {
    std::cout << p->id << " : " << p->type;
    return 0;
}

int PrintVisitor::visit(FunDec* fundec) {
    cout << "fun " << fundec->id << '(';
    fundec->paramList->accept(this);
    cout << ')';
    if (!fundec->type.empty() && fundec->type != "Unit")
        cout << ": " << fundec->type;
    cout << " {\n";
    fundec->block->accept(this);
    cout << "}";
    return 0;
}

int PrintVisitor::visit(FunDecList* fl) {
    for (const auto& f : fl->fundecs) {
        f->accept(this);
        if (f != fl->fundecs.back()) {
            cout << '\n';
        }
    }
    return 0;
}

int PrintVisitor::visit(FCallExp* fcall) {
    cout << fcall->nombre << '(';
    bool first = true;
    for (const auto& e : fcall->argumentos) {
        if (!first) {
            std::cout << ", ";
        }
        e->accept(this);
        first = false;
    }
    cout << ")";
    return 0;
}

int PrintVisitor::visit(FCallStm* stm) {
    cout << stm->nombre << "( ";
    bool first = true;
    for (auto e : stm->argumentos) {
        if (!first) {
            std::cout << ", ";
        }
        e->accept(this);
        first = false;
    }
    cout << ")" << endl;
    return 0;
}

int PrintVisitor::visit(ClassDec* cd) {
    cout << "class " << cd->id << "(";
    cd->pl->accept(this);
    cout << ") {\n";
    cd->vdl->accept(this);
    cout << "}\n";
    return 0;
}

int PrintVisitor::visit(ClassDecList* cdl) {
    for (const auto& cd : cdl->classdecs) {
        cd->accept(this);
        cout << '\n';
    }
    return 0;
}

int PrintVisitor::visit(ClassAccessor* ca) {
    cout << ca->object << "." << ca->parameter;
    return 0;
}