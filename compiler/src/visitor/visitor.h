#ifndef VISITOR_H
#define VISITOR_H
#include <iostream>
#include <string>
#include "environment.h"
#include "../exp/exp.h"

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
class ParamList;
class Param;
class FCallStm;
class ClassAccessor;

class Visitor {
public:
    virtual ~Visitor() = default;
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
    virtual int visit(ParamList* pl) = 0;
    virtual int visit(Param* p) = 0;
    virtual int visit(FunDec* f) = 0;
    virtual int visit(FunDecList* fl) = 0;
    virtual int visit(FCallExp* fcall) = 0;
    virtual int visit(Program* p) = 0;
    virtual int visit(FCallStm* p) = 0;
    virtual int visit(ClassDec* p) = 0;
    virtual int visit(ClassDecList* p) = 0;
    virtual int visit(ClassAccessor* p) = 0;
};

class PrintVisitor final : public Visitor {
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
    int visit(ParamList* pl) override;
    int visit(Param* p) override;
    int visit(FunDec* f) override;
    int visit(FunDecList* fl) override;
    int visit(FCallExp* fcall) override;
    int visit(Program* p) override;
    int visit(FCallStm* p) override;
    int visit(ClassDec* p) override;
    int visit(ClassDecList* p) override;
    int visit(ClassAccessor* ca) override;
};

struct ClassInfo {
    int size;
    unordered_map<string, int> off;
};

class GenCodeVisitor final : public Visitor {
    bool recolectando = false;
    std::ostream& out;

public:
    explicit GenCodeVisitor(std::ostream& out) : out(out) {
    }

    void generar(Program* program);
    unordered_map<string, int> memoria;
    unordered_map<string, bool> memoriaGlobal;
    unordered_map<string, ClassInfo> clases;
    unordered_map<string, string> tipoClase;
    int offset = -8;
    int whilelabelcont = 0;
    int forlabelcont = 0;
    int iflabelcont = 0;
    bool entornoClase = false;
    bool entornoFuncion = false;
    string nombreFuncion;
    string nombreClase;
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
    int visit(ParamList* pl) override;
    int visit(Param* p) override;
    int visit(FunDec* f) override;
    int visit(FunDecList* fl) override;
    int visit(FCallExp* fcall) override;
    int visit(Program* p) override;
    int visit(FCallStm* p) override;
    int visit(ClassDecList* p) override;
    int visit(ClassDec* p) override;
    int visit(ClassAccessor* ca) override;
};

#endif  // VISITOR_H
