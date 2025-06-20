#ifndef PARSER_H
#define PARSER_H

#include "exp/exp.h"
#include "iostream"
#include "scanner/scanner.h"

class Parser {
   private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    list<Stm*> parseStmList();
    Exp* parseExpression();
    Exp* parseLogicAnd();
    Exp* parseEquality();
    Exp* parseComparison();
    Exp* parseTerm();
    Exp* parseFactor();
    Exp* parseUnary();
    Exp* parsePrimary();
    static void errorHandler(string token, string regla) {
        std::cout << "Error: se esperaba un " << token << " en " << regla << std::endl;
        exit(1);
    }
    void consumeENDL();
    vector<Exp*> parseArguments();
    IfStatement* handleIfStatement();
    PrintStatement* handlePrintStatement();
    ForStatement* handleForStatement();
    WhileStatement* handleWhileStatement();
    AssignStatement* handleAssignStatement();
    VarDec* handleVarDecWithImplicitType(bool is_mut, string name);
    VarDec* handleVarDecWithExplicitType(bool is_mut, string name);

   public:
    Parser(Scanner* scanner);
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