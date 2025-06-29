#include "exp.h"
#include <iostream>

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op) : left(l), right(r), op(op) {
    if (op == PLUS_OP || op == MINUS_OP || op == MUL_OP || op == DIV_OP) {
        type = "int";
    } else {
        type = "bool";
    }
}

NumberExp::NumberExp(int v) : value(v) {
}

BoolExp::BoolExp(bool v) : value(v) {
}

IdentifierExp::IdentifierExp(const string& n) : name(n) {
}

Exp::~Exp() {
}

BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}

NumberExp::~NumberExp() = default;

BoolExp::~BoolExp() = default;

IdentifierExp::~IdentifierExp() = default;

UnaryExp::UnaryExp(Exp* e, UnaryOp o) : exp(e), op(o) {
};

UnaryExp::~UnaryExp() {
    delete exp;
}

AssignStatement::AssignStatement(std::string id, Exp* e) : id(std::move(id)), rhs(e) {
}

AssignStatement::~AssignStatement() {
    delete rhs;
}

PrintStatement::PrintStatement(Exp* e, std::string t) : type(std::move(t)), exp(e) {
}

PrintStatement::~PrintStatement() {
    delete exp;
}

IfStatement::IfStatement(Exp* c, Block* t, Block* e) : condition(c), then(t), els(e) {
}

IfStatement::~IfStatement() {
    delete condition;
    delete then;
    delete els;
}

Param::~Param() = default;

Param::Param(std::string i, std::string t) : id(std::move(i)), type(std::move(t)) {
};

ParamList::~ParamList() {
    for (const auto& p : param_list) {
        delete p;
    }
}

ForStatement::~ForStatement() {
    delete begin;
    delete end;
    delete block;
}

ReturnStatement::ReturnStatement(Exp* r) : ret(r) {
}

ReturnStatement::~ReturnStatement() {
    delete ret;
}

WhileStatement::WhileStatement(Exp* c, Block* b) : condition(c), block(b) {
}

WhileStatement::~WhileStatement() {
    delete condition;
    delete block;
}

VarDec::~VarDec() {
    delete stm;
}

VarDecList::~VarDecList() {
    for (const auto& v : vardecs) {
        delete v;
    }
}

void VarDecList::add(VarDec* vardec) {
    vardecs.push_back(vardec);
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
    delete vl;
    delete fl;
    delete cl;
    delete stml;
}

Stm::~Stm() = default;

std::string Exp::binopToString(const BinaryOp op) {
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
    delete pl;
    delete vdl;
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
    for (const auto& f : fundecs) {
        delete f;
    }
}