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
    program->vl->accept(this);
    program->stml->accept(this);
    return 0;
};

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
    cout << endl;
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
    return 0;
}

int PrintVisitor::visit(UnaryExp* e) {
    e->op == UnaryOp::NOT_OP ? cout << "!" : cout << "-";
    e->exp->accept(this);
    return 0;
}