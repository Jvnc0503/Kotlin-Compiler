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
        string var = previous->text;
        if (match(Token::LPAREN)) {
            return handleFCallStm(var);
        } else if (match(Token::ASSIGN)) {
            return handleAssignStatement(var);
        } else {
            errorHandler("ASSIGN O LPAREN", "ID");
        }
    } else if (match(Token::FOR)) {
        return handleForStatement();
    } else if (match(Token::WHILE)) {
        return handleWhileStatement();
    } else if (match(Token::RETURN)) {
        Exp* e = parseExpression();
        return new ReturnStatement(e);
    }

    return stm;
}

FCallStm* Parser::handleFCallStm(string nombre) {
    FCallStm* fc = new FCallStm();
    vector<Exp*> lista;
    if (match(Token::RPAREN)) {
        fc->argumentos = lista;
        fc->nombre = nombre;
        return fc;
    }
    lista.push_back(parseExpression());
    while (match(Token::COMMA)) {
        lista.push_back(parseExpression());
    }
    if (!match(Token::RPAREN)) {
        cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
        exit(1);
    }
    fc->argumentos = lista;
    fc->nombre = nombre;
    return fc;
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
        errorHandler("RPAREN", "PRINT");
    }
    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "PRINT");
    }
    consumeENDL();
    return new PrintStatement(e, type);
}

AssignStatement* Parser::handleAssignStatement(string nombre) {
    Exp* e = nullptr;
    e = parseExpression();
    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "ASSIGN");
    }
    consumeENDL();
    return new AssignStatement(nombre, e);
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
    BinaryExp* sum = new BinaryExp(id, number, PLUS_OP);
    AssignStatement* step = new AssignStatement(var, sum);

    fstm->step = step;
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