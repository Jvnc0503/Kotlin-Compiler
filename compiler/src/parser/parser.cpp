#include "parser.h"
#include <iostream>
#include <stdexcept>
#include "exp/exp.h"
#include "scanner/scanner.h"
#include "scanner/token.h"

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
    p->cl = parseClassDecList();
    consumeENDL();
    p->fl = parseFunDecList();
    consumeENDL();
    return p;
}
