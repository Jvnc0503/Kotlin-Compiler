#ifndef VISITOR_H
#define VISITOR_H
#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <vector>

#include "exp.h"

class BinaryExp;
class NumberExp;
class BoolExp;
class IdentifierExp;
class UnaryExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class VarDec;
class VarDecList;
class StmtList;
class Block;
class Program;
class ForStatement;
class WhileStatement;
class ReturnStatement;

class Visitor {
   public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(BoolExp* exp) = 0;
    virtual int visit(UnaryExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(AssignStatement* stm) = 0;
    virtual int visit(PrintStatement* stm) = 0;
    virtual int visit(IfStatement* stm) = 0;
    virtual int visit(VarDec* stm) = 0;
    virtual int visit(VarDecList* stm) = 0;
    virtual int visit(StmtList* stm) = 0;
    virtual int visit(Block* b) = 0;
    virtual int visit(ForStatement* stm) = 0;
    virtual int visit(WhileStatement* stm) = 0;
    virtual int visit(ReturnStatement* stm) = 0;
};

class PrintVisitor : public Visitor {
   public:
    int imprimir(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(AssignStatement* stm) override;
    int visit(PrintStatement* stm) override;
    int visit(IfStatement* stm) override;
    int visit(VarDec* stm) override;
    int visit(VarDecList* stm) override;
    int visit(StmtList* stm) override;
    int visit(Block* b) override;
    int visit(ForStatement* stm) override;
    int visit(WhileStatement* stm) override;
    int visit(ReturnStatement* stm) override;
    int visit(UnaryExp* exp) override;
};

class TypeCheckerVisitor : public Visitor {
   public:
    int revisar(Program* program);
    std::vector<std::string> variables;
    int cantidad = 0;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(UnaryExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(AssignStatement* stm) override;
    int visit(PrintStatement* stm) override;
    int visit(IfStatement* stm) override;
    int visit(VarDec* stm) override;
    int visit(VarDecList* stm) override;
    int visit(StmtList*) override;
    int visit(Block* b) override;
    int visit(ForStatement* stm) override;
    int visit(WhileStatement* stm) override;
    int visit(ReturnStatement* stm) override;
};

class GenCodeVisitor : public Visitor {
   public:
    std::unordered_map<std::string, int> memoria;
    int cantidad;
    int for_count;
    std::stack<int> for_stack;
    int gencode(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(UnaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(AssignStatement* stm) override;
    int visit(PrintStatement* stm) override;
    int visit(IfStatement* stm) override;
    int visit(VarDec* stm) override;
    int visit(VarDecList* stm) override;
    int visit(StmtList* stm) override;
    int visit(Block* b) override;
    int visit(ForStatement* stm) override;
    int visit(WhileStatement* stm) override;
    int visit(ReturnStatement* stm) override;
};

#endif  // VISITOR_H