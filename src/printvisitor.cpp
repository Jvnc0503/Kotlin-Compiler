#include "visitor.h"

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToString(exp->op) << ' ';
    exp->right->accept(this);
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
    cout << ";";
    return 0;
}

int PrintVisitor::visit(PrintStatement* stm) {
    if (stm->type == "print") {
        cout << "print(";
        stm->exp->accept(this);
        cout << ")";
    } else {
        cout << "println(";
        stm->exp->accept(this);
        cout << ")" << endl;
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
    if (program->vl)   program->vl->accept(this);
    if (program->fl)   program->fl->accept(this);   
    return 0;
}
int PrintVisitor::visit(VarDec* stm) {
    stm->is_mut ? cout << "var " : cout << "val ";
    cout << stm->var;
    cout << " :" << stm->type;
    if (stm->exp != nullptr) {
        cout << " = ";
        stm->exp->accept(this);
    };
    return 0;
}

int PrintVisitor::visit(VarDecList* stm) {
    for (auto i : stm->vardecs) {
        i->accept(this);
        cout << endl;
    }
    return 0;
}

int PrintVisitor::visit(StmtList* stm) {
    for (auto i : stm->stm_list) {
        i->accept(this);
        cout << endl;
    }
    return 0;
}

int PrintVisitor::visit(Block* stm) {
    stm->vardecl->accept(this);
    stm->stmdecl->accept(this);
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
    cout << "return "; 
    stm->ret->accept(this);
    return 0;
}

int PrintVisitor::visit(UnaryExp* e) {
    e->op == UnaryOp::NOT_OP ? cout << "!" : cout << "-";
    e->exp->accept(this);
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
    cout << "fun " << fundec->id << " (";
    fundec->paramList->accept(this);
    cout << ") :" << fundec->type << " {" << endl;
    fundec->block->accept(this);
    cout << "}";
    return 0;
}

int PrintVisitor::visit(FunDecList* fl) {
    for (auto f : fl->fundecs) {
        f->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(FCallExp* exp) {
    cout << exp->nombre << "( ";
    bool first = true;
    for (auto e : exp->argumentos) {
        if (!first) {
            std::cout << ", ";
        }
        e->accept(this);
        first = false;
    }
    cout << ")" << endl;
    return 0;
}
