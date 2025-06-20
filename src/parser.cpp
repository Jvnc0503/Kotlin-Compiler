#include "parser.h"
#include <iostream>
#include <stdexcept>
#include "exp.h"
#include "scanner.h"
#include "token.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    } else {
        return false;
    }
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) {
        return false;
    } else {
        return current->type == ttype;
    }
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) {
            delete previous;
        }
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERROR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    } else {
        return false;
    }
}

bool Parser::isAtEnd() {
    return current->type == Token::ENDOFFILE;
}

void Parser::consumeENDL() {
    while (check(Token::ENDL)) {
        advance();
    }
}

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERROR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

Program* Parser::parseProgram() {
    consumeENDL();
    Program* p = new Program();
    p->vl = parseVarDecList();
    consumeENDL();
    p->fl = parseFunDecList();
    consumeENDL();
    return p;
}

VarDecList* Parser::parseVarDecList() {
    consumeENDL();
    VarDecList* vdl = new VarDecList();
    VarDec* aux = parseVarDec();
    while (aux) {
        vdl->add(aux);
        consumeENDL();
        aux = parseVarDec();
    }
    return vdl;
}

VarDec* Parser::parseVarDec() {
    consumeENDL();
    if (!(check(Token::VAL) || check(Token::VAR))) {
        return nullptr;
    } else {
        VarDec* vd = new VarDec();
        if (match(Token::VAL)) {
            vd->is_mut = false;
        } else if (match(Token::VAR)) {
            vd->is_mut = true;
        }
        if (!match(Token::ID)) {
            errorHandler("ID", "VarDec");
        }
        vd->var = previous->text;
        string var = previous->text;
        if (!match(Token::COLON)) {
            errorHandler("':'", "VarDec");
        }
        if (match(Token::BOOL_TYPE)) {
            vd->type = "Boolean";
        } else if (match(Token::INT_TYPE)) {
            vd->type = "Int";
        } else if (match(Token::UNIT_TYPE)) {
            vd->type = "Unit";
        } else {
            cout << "Error: se esperaba un tipo válido después de ':'" << endl;
            exit(1);
        }
        if (match(Token::ASSIGN)) {
            Exp* e = parseExpression();
            AssignStatement* stm = new AssignStatement(var, e);
            vd->stm = stm;
        }
        if (!match(Token::ENDL)) {
            errorHandler("ENDL", "VarDec");
        }
        consumeENDL();
        return vd;
    }
}

FunDecList* Parser::parseFunDecList() {
    consumeENDL();
    FunDecList* fdl = new FunDecList();

    FunDec* aux = parseFunDec();
    while (aux) {
        fdl->add(aux);
        consumeENDL();
        aux = parseFunDec();
    }
    return fdl;
}

FunDec* Parser::parseFunDec() {
    consumeENDL();
    if (!check(Token::FUN)) {
        return nullptr;
    }
    FunDec* fundec = new FunDec();
    if (!match(Token::FUN)) {
        errorHandler("FUN", "FUNDEC");
    }
    if (!match(Token::ID)) {
        errorHandler("ID", "FUNDEC");
    }
    string id = previous->text;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "FUNDEC");
    }
    ParamList* paramlist = parseParamList();
    if (!match(Token::RPAREN)) {
        errorHandler("RPAREN", "FUNDEC");
    }
    if (!match(Token::COLON)) {
        errorHandler("COLON", "FUNDEC");
    }
    if (!match(Token::INT_TYPE) || match(Token::BOOL_TYPE) || match(Token::UNIT_TYPE)) {
        errorHandler("TYPE", "FUNDEC");
    }
    string type = previous->text;
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "FUNDEC");
    }
    Block* block = parseBlock();
    if (!match(Token::RBRACE)) {
        cout << current->text << endl;
        errorHandler("RBRACE", "FUNDEC");
    }
    fundec->block = block;
    fundec->paramList = paramlist;
    fundec->id = id;
    fundec->type = type;
    return fundec;
}

ParamList* Parser::parseParamList() {
    ParamList* pl = new ParamList();

    Param* param = parseParam();
    if (!param) {
        return pl;
    }
    pl->add(param);
    while (match(Token::COMMA)) {
        consumeENDL();
        param = parseParam();
        if (!param) {
            errorHandler("PARAM", "PARAMLIST");
        }
        pl->add(param);
    }
    return pl;
}
Param* Parser::parseParam() {
    if (match(Token::ID)) {
        string id = previous->text;
        if (!match(Token::COLON)) {
            errorHandler("COLON", "PARAM");
        }
        if (!match(Token::ID)) {
            errorHandler("ID", "PARAM");
        }
        string type = previous->text;
        return new Param(id, type);
    }
    return nullptr;
}

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
    Exp* e = nullptr;
    Block* tb = nullptr;
    Block* eb = nullptr;

    if (match(Token::IF)) {
        if (!match(Token::LPAREN)) {
            errorHandler("LPAREN", "IF");
        }
        e = parseExpression();
        if (!match(Token::RPAREN)) {
            errorHandler("RPAREN", "IF");
        }
        if (!match(Token::LBRACE)) {
            errorHandler("LBRACE", "IF");
        }
        tb = parseBlock();
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
    } else if (match(Token::PRINT)) {
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
        return new PrintStatement(e, "print");
    } else if (match(Token::PRINTLN)) {
        if (!match(Token::LPAREN)) {
            errorHandler("LPAREN", "PRINT");
        }
        e = parseExpression();
        if (!match(Token::RPAREN)) {
            errorHandler("RPAREN", "PRINT");
        }
        if (!match(Token::ENDL)) {
            errorHandler("ENDL", "PRINTLN");
        }
        consumeENDL();
        return new PrintStatement(e, "println");
    } else if (match(Token::ID)) {
        string var = previous->text;
        if (!match(Token::ASSIGN)) {
            errorHandler("ASSIGN", "STATEMENT");
        }
        e = parseExpression();
        if (!match(Token::ENDL)) {
            errorHandler("ENDL", "ASSIGN");
        }
        consumeENDL();
        return new AssignStatement(var, e);
    } else if (match(Token::FOR)) {
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
        fstm->end = end;
        fstm->block = block;
        return fstm;
    } else if (match(Token::WHILE)) {
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
    } else if (match(Token::RETURN)) {
        Exp* e = parseExpression();
        return new ReturnStatement(e);
    }

    else {
        return stm;
    }
}

Exp* Parser::parseExpression() {
    Exp* left = parseLogicAnd();
    if (match(Token::OR)) {
        BinaryOp op = BinaryOp::OR_OP;
        Exp* right = parseLogicAnd();
        return new BinaryExp(left, right, op);
    } else {
        return left;
    }
}

Exp* Parser::parseLogicAnd() {
    Exp* left = parseEquality();
    if (match(Token::AND)) {
        BinaryOp op = BinaryOp::AND_OP;
        Exp* right = parseEquality();
        return new BinaryExp(left, right, op);
    } else {
        return left;
    }
}

Exp* Parser::parseEquality() {
    Exp* left = parseComparison();
    if (match(Token::EQEQ) || match(Token::NEQ)) {
        BinaryOp op;
        if (previous->text == "==") {
            op = BinaryOp::EQ_OP;
        } else {
            op = BinaryOp::NE_OP;
        }
        Exp* right = parseEquality();
        return new BinaryExp(left, right, op);
    } else {
        return left;
    }
}

Exp* Parser::parseComparison() {
    Exp* left = parseTerm();
    if (match(Token::LT) || match(Token::LTE) || match(Token::GTE) || match(Token::GT)) {
        BinaryOp op;
        switch (previous->type) {
            case Token::LT:
                op = LT_OP;
                break;
            case Token::LTE:
                op = LE_OP;
                break;
            case Token::GT:
                op = GT_OP;
                break;
            case Token::GTE:
                op = GE_OP;
                break;
            default:
                op = GE_OP;  // nunca debería caer aquí
        }
        Exp* right = parseTerm();
        return new BinaryExp(left, right, op);
    } else {
        return left;
    }
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    if (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->text == "+") {
            op = BinaryOp::PLUS_OP;
        } else {
            op = BinaryOp::MINUS_OP;
        }
        Exp* right = parseFactor();
        return new BinaryExp(left, right, op);
    } else {
        return left;
    }
}

Exp* Parser::parseFactor() {
    Exp* left = parseUnary();
    if (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->text == "*") {
            op = BinaryOp::MUL_OP;
        } else {
            op = BinaryOp::DIV_OP;
        }
        Exp* right = parseUnary();
        return new BinaryExp(left, right, op);
    } else {
        return left;
    }
}

Exp* Parser::parseUnary() {
    if (match(Token::NOT) || match(Token::MINUS)) {
        UnaryOp op;
        if (previous->text == "!") {
            op = UnaryOp::NOT_OP;
        } else {
            op = UnaryOp::NEG_OP;
        }
        Exp* exp = parsePrimary();
        return new UnaryExp(exp, op);
    } else {
        return parsePrimary();
    }
}

Exp* Parser::parsePrimary() {
    if (match(Token::INT_LITERAL)) {
        return new NumberExp(stoi(previous->text));
    } else if (match(Token::TRUE)) {
        return new BoolExp(1);
    } else if (match(Token::FALSE)) {
        return new BoolExp(0);
    } else if (match(Token::ID)) {
        string nombre = previous->text;
        if (match(Token::LBRACE)) {
            FCallExp* fcall = new FCallExp();
            fcall->nombre = nombre;
            fcall->argumentos = parseArguments();
            return fcall;
        } else {
            return new IdentifierExp(nombre);
        }

    } else if (match(Token::LPAREN)) {
        Exp* e = parseExpression();
        if (!match(Token::RPAREN)) {
            errorHandler("RPAREN", "PRIMARY");
        }
        return e;
    } else {
        return nullptr;
    }
}

vector<Exp*> Parser::parseArguments() {
    vector<Exp*> arguments;
    Exp* aux = parseExpression();
    while (aux) {
        arguments.push_back(aux);
        aux = parseExpression();
    }
    return arguments;
}