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
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd())
        return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous)
            delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERROR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::ENDOFFILE);
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
    Program* p = new Program();
    p->vl = parseVarDecList();
    p->stml = parseStatementList();
    //    p->fl = parseFunDecList();
    //    p->cl = parseClassDecList();
    return p;
}

VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();
    VarDec* aux = parseVarDec();
    while (aux != nullptr) {
        vdl->add(aux);
        aux = parseVarDec();
    }
    return vdl;
}

// FunDecList* Parser::parseFunDecList() {
//    FunDecList* fdl = new FunDecList();
//    FunDec* aux = parseFunDec();
//    while (aux != nullptr) {
//       fdl->add(aux);
//       aux = parseFunDec();
//    }
//    return fdl;
// }

VarDec* Parser::parseVarDec() {
    if (!(check(Token::VAL) || check(Token::VAR))) {
        cout << "retornando nullptr "
             << "current: " << current->type << endl;
        return nullptr;
    }

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
        vd->exp = e;
    }

    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "VarDec");
    }

    return vd;
}

StmtList* Parser::parseStatementList() {
    StmtList* sl = new StmtList();
    Stm* aux = parseStatement();
    while (aux != nullptr) {
        sl->add(aux);
        aux = parseStatement();
    }
    return sl;
}

Block* Parser::parseBlock() {
    Block* block = new Block();
    block->vardecl = parseVarDecList();
    block->stmdecl = parseStatementList();
    return block;
}

Stm* Parser::parseStatement() {
    Stm* stm = nullptr;
    Exp* e = nullptr;
    Block* tb = nullptr;
    Block* eb = nullptr;

    if (match(Token::IF)) {
        IfStatement* stm = new IfStatement();
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
        return new AssignStatement(var, e);
    }
    return stm;
}

Exp* Parser::parseExpression() {
    Exp* left = parseLogicAnd();
    if (match(Token::OR)) {
        BinaryOp op = BinaryOp::OR_OP;
        Exp* right = parseLogicAnd();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseLogicAnd() {
    Exp* left = parseEquality();
    if (match(Token::AND)) {
        BinaryOp op = BinaryOp::AND_OP;
        Exp* right = parseEquality();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseEquality() {
    Exp* left = parseComparison();
    if (match(Token::EQEQ) || match(Token::NEQ)) {
        BinaryOp op;
        previous->text == "==" ? op = BinaryOp::EQ_OP : op = BinaryOp::NE_OP;
        Exp* right = parseEquality();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseComparison() {
    Exp* left = parseTerm();
    if (match(Token::LT) || match(Token::LTE) || match(Token::GTE) || match(Token::GT)) {
        BinaryOp op;
        string op_s = previous->text;
        if (op_s == "<") {
            op = BinaryOp::LT_OP;
        } else if (op_s == "<=") {
            op = BinaryOp::LE_OP;
        } else if (op_s == ">") {
            op = BinaryOp::GT_OP;
        } else if (op_s == ">=") {
            op = BinaryOp::GE_OP;
        }
        Exp* right = parseTerm();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    if (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        previous->text == "+" ? op = BinaryOp::PLUS_OP : op = BinaryOp::MINUS_OP;
        Exp* right = parseFactor();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* left = parseUnary();
    if (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        previous->text == "*" ? op = BinaryOp::MUL_OP : op = BinaryOp::DIV_OP;
        Exp* right = parseUnary();
        return new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseUnary() {
    if (match(Token::NOT) || match(Token::MINUS)) {
        UnaryOp op;
        previous->text == "!" ? op = UnaryOp::NOT_OP : op = UnaryOp::NEG_OP;
        Exp* exp = parsePrimary();
        return new UnaryExp(exp, op);
    }
    return parsePrimary();
}

Exp* Parser::parsePrimary() {
    Exp* e = nullptr;
    if (match(Token::INT_LITERAL)) {
        return new NumberExp(stoi(previous->text));
    } else if (match(Token::TRUE)) {
        return new BoolExp(1);
    } else if (match(Token::FALSE)) {
        return new BoolExp(0);
    } else if (match(Token::ID)) {
        string nombre = previous->text;
        return new IdentifierExp(nombre);
    } else if (match(Token::LPAREN)) {
        Exp* e = parseExpression();
        if (!match(Token::RPAREN)) {
            errorHandler("RPAREN", "PRIMARY");
        }
    }
    return e;
}
