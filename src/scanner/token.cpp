#include "token.h"

#include <ostream>
#include <unordered_map>

Token::Token(const Type type, const int line, const int column) : type(type), line(line), column(column) {}

Token::Token(const Type type, const char c, const int line, const int column) : type(type), text(std::string(1, c)), line(line), column(column) {}

Token::Token(const Type type, const std::string& source, const int& first, const int& length, const int line, const int column)
    : type(type), text(source.substr(first, length)), line(line), column(column) {}

std::string Token::typeToString(const Type& type) {
    static const std::unordered_map<Type, std::string> types = {{PLUS, "PLUS"},
                                                                {MINUS, "MINUS"},
                                                                {MUL, "MUL"},
                                                                {DIV, "DIV"},
                                                                {LT, "LT"},
                                                                {GT, "GT"},
                                                                {LTE, "LTE"},
                                                                {GTE, "GTE"},
                                                                {EQEQ, "EQEQ"},
                                                                {NEQ, "NEQ"},
                                                                {AND, "AND"},
                                                                {OR, "OR"},
                                                                {NOT, "NOT"},
                                                                {ASSIGN, "ASSIGN"},
                                                                {COLON, "COLON"},
                                                                {SEMICOLON, "SEMICOLON"},
                                                                {DOT, "DOT"},
                                                                {COMMA, "COMMA"},
                                                                {RANGE, "RANGE"},
                                                                {ENDL, "ENDL"},
                                                                {LPAREN, "LPAREN"},
                                                                {RPAREN, "RPAREN"},
                                                                {LBRACE, "LBRACE"},
                                                                {RBRACE, "RBRACE"},
                                                                {IF, "IF"},
                                                                {ELSE, "ELSE"},
                                                                {WHILE, "WHILE"},
                                                                {FOR, "FOR"},
                                                                {BREAK, "BREAK"},
                                                                {CONTINUE, "CONTINUE"},
                                                                {RETURN, "RETURN"},
                                                                {IN, "IN"},
                                                                {VAR, "VAR"},
                                                                {VAL, "VAL"},
                                                                {FUN, "FUN"},
                                                                {CLASS, "CLASS"},
                                                                {THIS, "THIS"},
                                                                {INT_TYPE, "INT_TYPE"},
                                                                {BOOL_TYPE, "BOOL_TYPE"},
                                                                {UNIT_TYPE, "UNIT_TYPE"},
                                                                {INT_LITERAL, "INT_LITERAL"},
                                                                {TRUE, "TRUE"},
                                                                {FALSE, "FALSE"},
                                                                {ID, "ID"},
                                                                {ENDOFFILE, "ENDOFFILE"},
                                                                {PRINT, "PRINT"},
                                                                {PRINTLN, "PRINTLN"},
                                                                {ERROR, "ERROR"}};
    return types.contains(type) ? types.at(type) : "UNKNOWN";
}

std::ostream& operator<<(std::ostream& os, const Token& t) {
    os << "TOKEN(" << Token::typeToString(t.type) << ", " << t.text << ")";
    os << "\tline " << t.line << ", column " << t.column << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token* t) {
    return os << *t;
}
