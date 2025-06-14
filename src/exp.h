#ifndef EXP_H
#define EXP_H

#include <list>
#include <string>
#include <unordered_map>

#include "visitor.h"
using namespace std;
enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, LT_OP, LE_OP, EQ_OP };

class Body;

class Exp {
 public:
  virtual int accept(Visitor* visitor) = 0;
  virtual ~Exp() = 0;
  static char binopToChar(BinaryOp op);
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

class AssignStatement : public Stm {
 public:
  std::string id;
  Exp* rhs;
  AssignStatement(std::string id, Exp* e);
  int accept(Visitor* visitor);
  ~AssignStatement();
};

class PrintStatement : public Stm {
 public:
  Exp* e;
  PrintStatement(Exp* e);
  int accept(Visitor* visitor);
  ~PrintStatement();
};

class ForStatement : public Stm {
 public:
  Exp* compare;
  Stm* assign;
  Stm* increment;
  Body* body;
  ForStatement(Exp* c, Stm* a, Stm* i, Body* b);
  int accept(Visitor* visitor);
  ~ForStatement();
};

class IfStatement : public Stm {
 public:
  Exp* condition;
  Body* then;
  Body* els;
  IfStatement(Exp* condition, Body* then, Body* els);
  int accept(Visitor* visitor);
  ~IfStatement();
};

class VarDec {
 public:
  string type;
  list<string> vars;
  VarDec(string type, list<string> vars);
  int accept(Visitor* visitor);
  ~VarDec();
};

class VarDecList {
 public:
  list<VarDec*> vardecs;
  VarDecList();
  void add(VarDec* vardec);
  int accept(Visitor* visitor);
  ~VarDecList();
};

class StatementList {
 public:
  list<Stm*> stms;
  StatementList();
  void add(Stm* stm);
  int accept(Visitor* visitor);
  ~StatementList();
};

class Body {
 public:
  VarDecList* vardecs;
  StatementList* slist;
  Body(VarDecList* vardecs, StatementList* stms);
  int accept(Visitor* visitor);
  ~Body();
};

class Program {
 public:
  Body* body;
  Program(Body* body);
  int contar() {
    int a = 0;
    for (auto i : body->vardecs->vardecs) {
      a += i->vars.size();
    }
    return a;
  }
  ~Program();
};

#endif
