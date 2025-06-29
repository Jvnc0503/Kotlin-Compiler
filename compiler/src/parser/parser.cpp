#include "parser.h"
#include <iostream>
#include "../exp/exp.h"
#include "../scanner/scanner.h"
#include "../scanner/token.h"

bool Parser::match(const Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(const Token::Type ttype) const {
    if (isAtEnd()) {
        return false;
    }
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERROR)) {
            std::cerr << "Error de análisis, carácter no reconocido: " << current->text << '\n';
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() const {
    return current->type == Token::ENDOFFILE;
}

void Parser::consumeENDL() {
    while (check(Token::ENDL)) {
        advance();
    }
}

Parser::Parser(Scanner* scanner) : scanner(scanner) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERROR) {
        std::cerr << "Error en el primer token: " << current->text << '\n';
        exit(1);
    }
}

Program* Parser::parseProgram() {
    consumeENDL();
    auto* p = new Program();
    p->vl = parseVarDecList();
    consumeENDL();
    p->cl = parseClassDecList();
    consumeENDL();
    p->fl = parseFunDecList();
    consumeENDL();
    return p;
}
