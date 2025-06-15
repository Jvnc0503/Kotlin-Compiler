#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include <list>
#include <stack>
#include <vector>
#include <string>

class BinaryExp;
class NumberExp;
class BoolExp;
class IdentifierExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class VarDec;
class VarDecList;
class StatementList;
class Body;
class Program;
class ForStatement;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(BoolExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(AssignStatement* stm) = 0;
    virtual int visit(PrintStatement* stm) = 0;
    virtual int visit(IfStatement* stm) = 0;
    virtual int visit(VarDec* stm) = 0;
    virtual int visit(VarDecList* stm) = 0;
    virtual int visit(StatementList* stm) = 0;
    virtual int visit(Body* b) = 0;
    virtual int visit(ForStatement* stm) = 0;
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
    int visit(StatementList* stm) override;
    int visit(Body* b) override;
    int visit(ForStatement* stm) override;
};


class TypeCheckerVisitor : public Visitor {
public:
    int revisar(Program* program);
    std::vector<std::string> variables;
    int cantidad=0;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(AssignStatement* stm) override;
    int visit(PrintStatement* stm) override;
    int visit(IfStatement* stm) override;
    int visit(VarDec* stm) override;
    int visit(VarDecList* stm) override;
    int visit(StatementList* stm) override;
    int visit(Body* b) override;
    int visit(ForStatement* stm) override;
};


class GenCodeVisitor : public Visitor {
public:
    TypeCheckerVisitor typeChecker;
    std::unordered_map<std::string, int> memoria;
    int cantidad;
    int for_count;
    std::stack<int> for_stack;
    int gencode(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(AssignStatement* stm) override;
    int visit(PrintStatement* stm) override;
    int visit(IfStatement* stm) override;
    int visit(VarDec* stm) override;
    int visit(VarDecList* stm) override;
    int visit(StatementList* stm) override;
    int visit(Body* b) override;
    int visit(ForStatement* stm) override;
};

#endif // VISITOR_H