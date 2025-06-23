#include "parser.h"

StmtList* Parser::parseStatementList() {
    consumeENDL();
    StmtList* sl = new StmtList();
    Stm* aux = parseStatement();
    while (aux) {
        sl->add(aux);
        consumeENDL();
        aux = parseStatement();
    }
    return sl;
}

Block* Parser::parseBlock() {
    consumeENDL();
    Block* block = new Block();
    block->vardecl = parseVarDecList();
    consumeENDL();
    block->stmdecl = parseStatementList();
    consumeENDL();
    return block;
}

Stm* Parser::parseStatement() {
    consumeENDL();
    Stm* stm = nullptr;
    if (match(Token::IF)) {
        return handleIfStatement();
    } else if (match(Token::PRINT) || match(Token::PRINTLN)) {
        return handlePrintStatement();
    } else if (match(Token::ID)) {
        return handleAssignStatement();
    } else if (match(Token::FOR)) {
        return handleForStatement();
    } else if (match(Token::WHILE)) {
        return handleWhileStatement();
    } else if (match(Token::RETURN)) {
        Exp* e = parseExpression();
        return new ReturnStatement(e);
    }

    else {
        return stm;
    }
}

IfStatement* Parser::handleIfStatement() {
    Block* eb;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "IF");
    }
    Exp* e = parseExpression();
    if (!match(Token::RPAREN)) {
        errorHandler("RPAREN", "IF");
    }
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "IF");
    }
    Block* tb = parseBlock();
    if (!match(Token::RBRACE)) {
        errorHandler("RBRACE", "IF");
    }
    if (match(Token::ELSE)) {
        if (!match(Token::LBRACE)) {
            errorHandler("LBRACE", "IF");
        }
        eb = parseBlock();
        if (!match(Token::RBRACE)) {
            errorHandler("RBRACE", "IF");
        }
    }
    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "IF");
    }
    consumeENDL();
    return new IfStatement(e, tb, eb);
}

PrintStatement* Parser::handlePrintStatement() {
    string type = previous->text;
    Exp* e = nullptr;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "PRINT");
    }
    e = parseExpression();
    if (!match(Token::RPAREN)) {
        cout << current->text << endl;
        errorHandler("RPAREN", "PRINT");
    }
    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "PRINT");
    }
    consumeENDL();
    return new PrintStatement(e, type);
}

AssignStatement* Parser::handleAssignStatement() {
    string var = previous->text;
    Exp* e = nullptr;
    if (!match(Token::ASSIGN)) {
        errorHandler("ASSIGN", "STATEMENT");
    }
    e = parseExpression();
    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "ASSIGN");
    }
    consumeENDL();
    return new AssignStatement(var, e);
}

ForStatement* Parser::handleForStatement() {
    Exp* e = nullptr;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "FOR");
    }
    if (!match(Token::ID)) {
        errorHandler("ID", "FOR");
    }
    string var = previous->text;
    if (!match(Token::IN)) {
        errorHandler("IN", "FOR");
    }
    e = parseExpression();
    AssignStatement* astm = new AssignStatement();
    astm->id = var;
    astm->rhs = e;
    if (!match(Token::RANGE)) {
        errorHandler("RANGE", "FOR");
    }
    Exp* end = parseExpression();
    IdentifierExp* id = new IdentifierExp(var);

    Exp* condition = new BinaryExp(id, end, LE_OP);
    if (!match(Token::RPAREN)) {
        errorHandler("RPAREN", "FOR");
    }
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "FOR");
    }
    Block* block = parseBlock();
    if (!match(Token::RBRACE)) {
        errorHandler("RBRACE", "FOR");
    }
    consumeENDL();
    ForStatement* fstm = new ForStatement();
    fstm->begin = astm;
    fstm->end = condition;
    fstm->block = block;

    NumberExp* number = new NumberExp(1);
    BinaryExp* sum = new BinaryExp(id,number, PLUS_OP );
    AssignStatement* step = new AssignStatement(var ,sum);

    fstm -> step = step;
    return fstm;
}

WhileStatement* Parser::handleWhileStatement() {
    Exp* e = nullptr;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "WHILE");
    }
    e = parseExpression();
    if (!match(Token::RPAREN)) {
        errorHandler("RPAREN", "WHILE");
    }
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "WHILE");
    }
    Block* block = parseBlock();
    if (!match(Token::RBRACE)) {
        errorHandler("RBRACE", "WHILE");
    }
    return new WhileStatement(e, block);
}