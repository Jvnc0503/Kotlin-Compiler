#ifndef EXP_H
#define EXP_H

#include <list>
#include <string>
#include <unordered_map>
#include <vector>

class Visitor;

using namespace std;
enum BinaryOp {
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    LT_OP,
    LE_OP,
    GT_OP,
    GE_OP,
    NE_OP,
    EQ_OP,
    OR_OP,
    AND_OP,
};
enum UnaryOp { NOT_OP, NEG_OP };

class FunDec;
class VarDec;
class Block;
class Programm;
class StmDec;
class VarDecList;
class StmtList;

class Exp {
   public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToString(BinaryOp op);
};

class BinaryExp : public Exp {
   public:
    Exp *left, *right;
    string type;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor);
    ~BinaryExp();
};

class UnaryExp : public Exp {
   public:
    Exp* exp;
    UnaryOp op;
    UnaryExp(Exp* e, UnaryOp op);
    ~UnaryExp();
    int accept(Visitor* visitor);
};

class NumberExp : public Exp {
   public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ~NumberExp();
};

class BoolExp : public Exp {
   public:
    int value;
    BoolExp(bool v);
    int accept(Visitor* visitor);
    ~BoolExp();
};

class IdentifierExp : public Exp {
   public:
    std::string name;
    IdentifierExp(const std::string& n);
    int accept(Visitor* visitor);
    ~IdentifierExp();
};

class Stm {
   public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class StmtList {
   public:
    list<Stm*> stm_list;
    StmtList(){};
    ~StmtList();
    void add(Stm* s) { stm_list.push_back(s); }
    int accept(Visitor* visitor);
};

class AssignStatement : public Stm {
   public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);
    AssignStatement(){};
    int accept(Visitor* visitor);
    ~AssignStatement();
};

class PrintStatement : public Stm {
   public:
    string type;
    Exp* exp;
    PrintStatement(Exp* e, string t);
    PrintStatement(){};
    int accept(Visitor* visitor);
    ~PrintStatement();
};

class ReturnStatement : public Stm {
   public:
    Exp* ret;
    ReturnStatement(Exp* r);
    ~ReturnStatement();
    int accept(Visitor* visitor);
};

class ForStatement : public Stm {
   public:
    AssignStatement* begin;
    Exp* end;
    Block* block;
    ForStatement(){};
    int accept(Visitor* visitor);
    ~ForStatement();
};

class WhileStatement : public Stm {
   public:
    Exp* condition;
    Block* block;
    WhileStatement(Exp* c, Block* b);
    ~WhileStatement();
    int accept(Visitor* visitor);
};

class IfStatement : public Stm {
   public:
    Exp* condition;
    Block* then;
    Block* els;
    IfStatement(Exp* condition, Block* then, Block* els);
    IfStatement(){};
    int accept(Visitor* visitor);
    ~IfStatement();
};

class Block {
   public:
    VarDecList* vardecl;
    StmtList* stmdecl;
    Block(){};
    ~Block();
    int accept(Visitor* visitor);
};

class Param {
   public:
    string id;
    string type;
    Param(string i, string t);
    ~Param();
    int accept(Visitor* visitor);
};

class ParamList {
   public:
    vector<Param*> param_list;
    ParamList() {}
    void add(Param* param) { param_list.push_back(param); };
    ~ParamList();
    int accept(Visitor* visitor);
};

class FCallExp : public Exp {
   public:
    string nombre;
    vector<Exp*> argumentos;
    FCallExp(){};
    ~FCallExp(){};
    int accept(Visitor* visitor);
};

class FunDec {
   public:
    string id;
    string type;
    ParamList* paramList;
    Block* block;
    FunDec(){};
    ~FunDec();
    int accept(Visitor* visitor);
};

class FunDecList {
   public:
    list<FunDec*> fundecs;
    void add(FunDec* fundec) { fundecs.push_back(fundec); };
    FunDecList(){};
    ~FunDecList();
    int accept(Visitor* visitor);
};

class VarDec {
   public:
    bool is_mut;
    string type;
    string var;
    AssignStatement* stm;
    VarDec(){};
    int accept(Visitor* visitor);
    ~VarDec();
};

class VarDecList {
   public:
    list<VarDec*> vardecs;
    void add(VarDec* vardec);
    VarDecList(){};
    ~VarDecList();
    int accept(Visitor* visitor);
};

class ClassDec {
   public:
    string id;
    list<VarDec*> vardecs;
    ClassDec(){};
    ~ClassDec();
    int accept(Visitor* visitor);
};

class ClassDecList {
   public:
    list<ClassDec*> classdecs;
    ClassDecList(){};
    ~ClassDecList();
    int accept(Visitor* visitor);
};

class Program {
   public:
    VarDecList* vl;
    FunDecList* fl;
    ClassDecList* cl;
    StmtList* stml;
    Program() : vl(nullptr), fl(nullptr), cl(nullptr), stml(nullptr) {}
    ~Program();
    int accept(Visitor* visitor);
};

#endif
