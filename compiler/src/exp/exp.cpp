#include "exp.h"

#include <iostream>

#include "scanner/token.h"

using namespace std;

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op) : left(l), right(r), op(op) {
    if (op == PLUS_OP || op == MINUS_OP || op == MUL_OP || op == DIV_OP) {
        type = "int";
    } else {
        type = "bool";
    }
}
NumberExp::NumberExp(int v) : value(v) {}
BoolExp::BoolExp(bool v) : value(v) {}
IdentifierExp::IdentifierExp(const string& n) : name(n) {}
Exp::~Exp() {}
BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}

NumberExp::~NumberExp() {}
BoolExp::~BoolExp() {}
IdentifierExp::~IdentifierExp() {}
UnaryExp::UnaryExp(Exp* e, UnaryOp o) : exp(e), op(o){};
UnaryExp::~UnaryExp() {
    delete exp;
}
AssignStatement::AssignStatement(string id, Exp* e) : id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(Exp* e, string t) : exp(e), type(t) {}
PrintStatement::~PrintStatement() {
    delete exp;
}

IfStatement::IfStatement(Exp* c, Block* t, Block* e) : condition(c), then(t), els(e) {}
IfStatement::~IfStatement() {
    delete condition;
    delete then;
    delete els;
}

Param::~Param() {}
Param::Param(string i, string t) : id(i), type(t){};

ParamList::~ParamList() {
    for (auto p : param_list) {
        delete p;
    }
}

ForStatement::~ForStatement() {
    delete begin;
    delete end;
    delete block;
}

ReturnStatement::ReturnStatement(Exp* r) : ret(r) {}
ReturnStatement::~ReturnStatement() {
    delete ret;
}

WhileStatement::WhileStatement(Exp* c, Block* b) : condition(c), block(b) {}
WhileStatement::~WhileStatement() {
    delete condition;
    delete block;
}

VarDec::~VarDec() {
    delete stm;
}

VarDecList::~VarDecList() {
    for (auto v : vardecs) {
        delete v;
    }
}
void VarDecList::add(VarDec* v) {
    vardecs.push_back(v);
}

StmtList::~StmtList() {
    for (auto s : stm_list) {
        delete s;
    }
}

Block::~Block() {
    delete vardecl;
    delete stmdecl;
}

Program::~Program() {
    if (vl)
        delete vl;
    if (fl)
        delete fl;
    if (cl)
        delete cl;
    if (stml)
        delete stml;
}
Stm::~Stm() {}
string Exp::binopToString(BinaryOp op) {
    switch (op) {
        case PLUS_OP:
            return "+";
        case MINUS_OP:
            return "-";
        case MUL_OP:
            return "*";
        case DIV_OP:
            return "/";
        case LT_OP:
            return "<";
        case LE_OP:
            return "<=";
        case GT_OP:
            return ">";
        case GE_OP:
            return ">=";
        case NE_OP:
            return "!=";
        case EQ_OP:
            return "==";
        case OR_OP:
            return "||";
        case AND_OP:
            return "&&";
        default:
            return "$";
    }
}

ClassDec::~ClassDec() {
    for (auto v : vardecs) {
        delete v;
    }
}

ClassDecList::~ClassDecList() {
    for (auto c : classdecs) {
        delete c;
    }
}

FunDec::~FunDec() {
    delete block;
}

FunDecList::~FunDecList() {
    for (auto f : fundecs) {
        delete f;
    }
}