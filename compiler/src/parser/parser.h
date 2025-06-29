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

    static void errorHandler(const string& token, const string& rule) {
        std::cerr << "Error: se esperaba un " << token << " en " << rule << '\n';
        exit(1);
    }

    void consumeENDL();
    vector<Exp*> parseArguments();
    IfStatement* handleIfStatement();
    PrintStatement* handlePrintStatement();
    ForStatement* handleForStatement();
    WhileStatement* handleWhileStatement();
    AssignStatement* handleAssignStatement(const string& nombre);
    FCallStm* handleFCallStm(string nombre);
    VarDec* handleVarDecWithImplicitType(bool is_mut, const string& name);
    VarDec* handleVarDecWithExplicitType(bool is_mut, const string& name);

public:
    explicit Parser(Scanner* scanner);
    Program* parseProgram();
    Stm* parseStatement();
    StmtList* parseStatementList();
    VarDec* parseVarDec();
    VarDecList* parseVarDecList();
    Block* parseBlock();
    FunDecList* parseFunDecList();
    FunDec* parseFunDec();
    ClassDecList* parseClassDecList();
    ClassDec* parseClassDec();
    ParamList* parseParamList();
    Param* parseParam();
};

#endif  // PARSER_H