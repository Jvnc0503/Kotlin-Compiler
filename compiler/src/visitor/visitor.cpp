//
// Created by m41k1 on 6/13/25.
//

#include "visitor.h"

int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int UnaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int ForStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int VarDec::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int VarDecList::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int StmtList::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Block::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int WhileStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int ReturnStatement::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int ParamList::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Param::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FunDec::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FunDecList::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FCallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FCallStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}