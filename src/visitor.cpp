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

int StatementList::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Body::accept(Visitor* visitor) {
    return visitor->visit(this);
}
