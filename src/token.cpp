#include "token.h"
#include <ostream>
#include <unordered_map>

Token::Token(const Type type) : type(type) {
}

Token::Token(const Type type, const char c) : type(type), text(std::string(1, c)) {
}

Token::Token(const Type type, const std::string &source, const size_t first, const size_t length) : type(type),
    text(source.substr(first, length)) {
}

std::string Token::typeToString(const Type &type) {
    static const std::unordered_map<Type, std::string> types = {
        {PLUS, "PLUS"}, {MINUS, "MINUS"}, {MUL, "MUL"}, {DIV, "DIV"},
        {LT, "LT"}, {GT, "GT"}, {LTE, "LTE"}, {GTE, "GTE"},
        {EQEQ, "EQEQ"}, {NEQ, "NEQ"},
        {AND, "AND"}, {OR, "OR"}, {NOT, "NOT"},
        {ASSIGN, "ASSIGN"}, {COLON, "COLON"}, {SEMICOLON, "SEMICOLON"},
        {DOT, "DOT"}, {COMMA, "COMMA"}, {RANGE, "RANGE"}, {ENDL, "ENDL"},
        {LPAREN, "LPAREN"}, {RPAREN, "RPAREN"}, {LBRACE, "LBRACE"},
        {RBRACE, "RBRACE"},
        {IF, "IF"}, {ELSE, "ELSE"}, {WHILE, "WHILE"}, {FOR, "FOR"},
        {BREAK, "BREAK"}, {CONTINUE, "CONTINUE"}, {RETURN, "RETURN"},
        {IN, "IN"},
        {VAR, "VAR"}, {VAL, "VAL"}, {FUN, "FUN"}, {CLASS, "CLASS"},
        {THIS, "THIS"},
        {INT_TYPE, "INT_TYPE"}, {BOOL_TYPE, "BOOL_TYPE"}, {UNIT_TYPE, "UNIT_TYPE"},
        {INT_LITERAL, "INT_LITERAL"}, {TRUE, "TRUE"}, {FALSE, "FALSE"},
        {ID, "ID"},
        {ENDOFFILE, "ENDOFFILE"}, {ERROR, "ERROR"}
    };
    return types.contains(type) ? types.at(type) : "UNKNOWN";
}

std::ostream &operator<<(std::ostream &os, const Token &t) {
    os << "TOKEN(" << Token::typeToString(t.type) << ", " << t.text << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Token *t) {
    return os << *t;
}
