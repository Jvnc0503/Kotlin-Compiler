#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "../scanner/token.h"
#include "../scanner/scanner.h"
#include "../exp/exp.h"

class Parser {
    Scanner* scanner;
    Token *previous, *current;
    bool isAtEnd() const;
    bool check(Token::Type ttype) const;
    bool advance();
    bool match(Token::Type ttype);
    Exp* parseExpression();
    Exp* parseLogicAnd();
    Exp* parseEquality();
    Exp* parseComparison();
    Exp* parseTerm();
    Exp* parseFactor();
    Exp* parseUnary();
    Exp* parsePrimary();
    Stm* parseStatement();
    StmtList* parseStatementList();
    VarDec* parseVarDec();
    VarDecList* parseVarDecList();
    Block* parseBlock();
    FunDecList* parseFunDecList();
    FunDec* parseFunDec();
    ClassDecList* parseClassDecList();
    ClassDec* parseClassDec();
    ParamList* parseFunParamList();
    Param* parseFunParam();
    ParamList* parseClassParamList();
    Param* parseClassParam();

    static void errorHandler(const std::string& token, const std::string& rule) {
        std::cerr << "Error: se esperaba un " << token << " en " << rule << '\n';
        exit(1);
    }

    void consumeENDL();
    void consumeSeparator();
    std::vector<Exp*> parseArguments();
    IfStatement* handleIfStatement();
    PrintStatement* handlePrintStatement();
    ForStatement* handleForStatement();
    WhileStatement* handleWhileStatement();
    AssignStatement* handleAssignStatement(const std::string& nombre);
    FCallStm* handleFCallStm(const std::string& nombre);
    VarDec* handleVarDecWithImplicitType(bool is_mut, const std::string& name);
    VarDec* handleVarDecWithExplicitType(bool is_mut, const std::string& name);

public:
    explicit Parser(Scanner* scanner);
    Program* parseProgram();
};

#endif  // PARSER_H