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
                op = GE_OP;  // nunca debería suceder
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
    } else if (match(Token::TRUE)) {
        return new BoolExp(1);
    } else if (match(Token::FALSE)) {
        return new BoolExp(0);
    } else if (match(Token::ID)) {
        string nombre = previous->text;
        if (match(Token::LPAREN)) {
            FCallExp* fcall = new FCallExp();
            fcall->nombre = nombre;
            fcall->argumentos = parseArguments();
            cout << current -> text << endl;
            if (!match(Token:: RPAREN)) {
                errorHandler("RPAREN", "FCALL");
            }
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
    vector<Exp*> args;

    if (check(Token::RPAREN))
        return args;

    do {
        Exp* e = parseExpression();   
        args.push_back(e);
    } while (match(Token::COMMA));    

    return args;                      
}