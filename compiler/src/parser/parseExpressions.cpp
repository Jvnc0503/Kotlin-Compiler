#include "parser.h"

Exp* Parser::parseExpression() {
    Exp* left = parseLogicAnd();
    while (match(Token::OR)) {
        Exp* right = parseLogicAnd();
        left = new BinaryExp(left, right, OR_OP);
    }
    return left;
}

Exp* Parser::parseLogicAnd() {
    Exp* left = parseEquality();
    while (match(Token::AND)) {
        Exp* right = parseEquality();
        left = new BinaryExp(left, right, AND_OP);
    }
    return left;
}

Exp* Parser::parseEquality() {
    Exp* left = parseComparison();
    while (match(Token::EQEQ) || match(Token::NEQ)) {
        BinaryOp op = (previous->type == Token::EQEQ) ? EQ_OP : NE_OP;
        Exp* right = parseComparison();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseComparison() {
    Exp* left = parseTerm();
    while (match(Token::LT) || match(Token::LTE) || match(Token::GT) || match(Token::GTE)) {
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
                op = GE_OP; // nunca debería suceder
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op = (previous->type == Token::PLUS) ? PLUS_OP : MINUS_OP;
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* left = parseUnary();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op = (previous->type == Token::MUL) ? MUL_OP : DIV_OP;
        Exp* right = parseUnary();
        left = new BinaryExp(left, right, op);
    }
    return left;
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
    }
    if (match(Token::TRUE)) {
        return new BoolExp(1);
    }
    if (match(Token::FALSE)) {
        return new BoolExp(0);
    }
    if (match(Token::ID)) {
        const std::string nombre = previous->text;
        if (match(Token::LPAREN)) {
            auto* fcall = new FCallExp();
            fcall->nombre = nombre;
            isupper(nombre[0]) ? fcall->is_class = true : fcall->is_class = false;
            if (!match(Token::RPAREN)) {
                fcall->argumentos = parseArguments();
                if (!match(Token::RPAREN)) {
                    errorHandler("RPAREN", "FCALL");
                }
            }
            return fcall;
        }
        if (match(Token::DOT)) {
            auto* ca = new ClassAccessor();
            if (!match(Token::ID)) {
                errorHandler("ID", "CLASS_ACCESSOR");
            }
            ca->object = nombre;
            ca->parameter = previous->text;
            return ca;
        }
        return new IdentifierExp(nombre);
    }
    if (match(Token::LPAREN)) {
        Exp* e = parseExpression();
        if (!match(Token::RPAREN)) {
            errorHandler("RPAREN", "PRIMARY");
        }
        return e;
    }
    return nullptr;
}

std::vector<Exp*> Parser::parseArguments() {
    std::vector<Exp*> args;

    if (check(Token::RPAREN))
        return args;

    do {
        Exp* e = parseExpression();
        args.push_back(e);
    } while (match(Token::COMMA));

    return args;
}