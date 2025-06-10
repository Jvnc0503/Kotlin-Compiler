#ifndef TOKEN_H
#define TOKEN_H


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
};


#endif //TOKEN_H
