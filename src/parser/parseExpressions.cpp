#include "parser.h"

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
                op = GE_OP;
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