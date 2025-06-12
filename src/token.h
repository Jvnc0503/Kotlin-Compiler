#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <ostream>

struct Token {
    enum Type {
        // Arithmetic operators
        PLUS, MINUS, MUL, DIV,

        // Comparison operators
        LT, GT, LTE, GTE, EQEQ, NEQ,

        // Bolean operators
        AND, OR, NOT,

        // Delimiters
        ASSIGN, COLON, SEMICOLON, DOT,
        COMMA, RANGE, ENDL,

        // Grouping
        LPAREN, RPAREN, LBRACE, RBRACE,

        // Control Structures
        IF, ELSE, WHILE, FOR, BREAK,
        CONTINUE, RETURN, IN,

        // Declarations
        VAR, VAL, FUN, CLASS,

        // References
        THIS,

        // Types
        INT_TYPE, BOOL_TYPE, UNIT_TYPE,

        // Values
        INT_LITERAL, TRUE, FALSE, ID,

        // Extra
        ENDOFFILE, ERROR
    };

    Type type;
    std::string text;
    int line;
    int column;

    Token(Type type, int line, int column);

    Token(Type type, char c, int line, int column);

    Token(Type type, const std::string &source, const int &first, const int &length, int line, int column);

    ~Token() = default;

    static std::string typeToString(const Type &type);

    friend std::ostream &operator<<(std::ostream &os, const Token &t);

    friend std::ostream &operator<<(std::ostream &os, const Token *t);
};

#endif //TOKEN_H
