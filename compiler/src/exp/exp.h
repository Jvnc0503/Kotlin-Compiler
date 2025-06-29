#ifndef EXP_H
#define EXP_H
#include <list>
#include <string>
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
class StmDec;
class VarDecList;
class StmtList;

class Exp {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;
    static string binopToString(BinaryOp op);
};

class BinaryExp final : public Exp {
public:
    Exp *left, *right;
    string type;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    int accept(Visitor* visitor) override;
    ~BinaryExp() override;
};

class UnaryExp final : public Exp {
public:
    Exp* exp;
    UnaryOp op;
    UnaryExp(Exp* e, UnaryOp op);
    ~UnaryExp() override;
    int accept(Visitor* visitor) override;
};

class NumberExp final : public Exp {
public:
    int value;
    explicit NumberExp(int v);
    int accept(Visitor* visitor) override;
    ~NumberExp() override;
};

class BoolExp final : public Exp {
public:
    int value;
    explicit BoolExp(bool v);
    int accept(Visitor* visitor) override;
    ~BoolExp() override;
};

class IdentifierExp final : public Exp {
public:
    std::string name;
    explicit IdentifierExp(std::string n);
    int accept(Visitor* visitor) override;
    ~IdentifierExp() override;
};

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

class StmtList {
public:
    list<Stm*> stm_list;

    StmtList() = default;
    ~StmtList();
    void add(Stm* s) { stm_list.push_back(s); }
    int accept(Visitor* visitor);
};

class AssignStatement : public Stm {
public:
    std::string id;
    Exp* rhs;
    AssignStatement(std::string id, Exp* e);

    AssignStatement(): rhs(nullptr) {
    }

    int accept(Visitor* visitor) override;
    ~AssignStatement() override;
};

class PrintStatement final : public Stm {
public:
    string type;
    Exp* exp;
    PrintStatement(Exp* e, string t);

    PrintStatement(): exp(nullptr) {
    }

    int accept(Visitor* visitor) override;
    ~PrintStatement() override;
};

class ReturnStatement final : public Stm {
public:
    Exp* ret;
    explicit ReturnStatement(Exp* r);
    ~ReturnStatement() override;
    int accept(Visitor* visitor) override;
};

class ForStatement final : public Stm {
public:
    AssignStatement* begin;
    Exp* end;
    AssignStatement* step;
    Block* block;

    ForStatement(): begin(nullptr), end(nullptr), step(nullptr), block(nullptr) {
    }

    int accept(Visitor* visitor) override;
    ~ForStatement() override;
};

class WhileStatement final : public Stm {
public:
    Exp* condition;
    Block* block;
    WhileStatement(Exp* c, Block* b);
    ~WhileStatement() override;
    int accept(Visitor* visitor) override;
};

class IfStatement final : public Stm {
public:
    Exp* condition;
    Block* then;
    Block* els;
    IfStatement(Exp* condition, Block* then, Block* els);

    IfStatement(): condition(nullptr), then(nullptr), els(nullptr) {
    }

    int accept(Visitor* visitor) override;
    ~IfStatement() override;
};

class Block {
public:
    VarDecList* vardecl;
    StmtList* stmdecl;

    Block(): vardecl(nullptr), stmdecl(nullptr) {
    }

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

    ParamList() = default;

    void add(Param* param) { param_list.push_back(param); };
    ~ParamList();
    int accept(Visitor* visitor);
};

class ClassAccessor final : public Exp {
public:
    string object;
    string parameter;

    ClassAccessor() = default;

    ~ClassAccessor() override = default;
    int accept(Visitor* visitor) override;
};

class FCallExp final : public Exp {
public:
    string nombre;
    vector<Exp*> argumentos;
    bool is_class;

    FCallExp(): is_class(false) {
    }

    ~FCallExp() override = default;
    int accept(Visitor* visitor) override;
};

class FCallStm final : public Stm {
public:
    string nombre;
    vector<Exp*> argumentos;

    FCallStm() = default;

    ~FCallStm() override = default;

    int accept(Visitor* visitor) override;
};

class FunDec {
public:
    string id;
    string type;
    ParamList* paramList;
    Block* block;

    FunDec(): paramList(nullptr), block(nullptr) {
    }

    ~FunDec();
    int accept(Visitor* visitor);
};

class FunDecList {
public:
    list<FunDec*> fundecs;
    void add(FunDec* fundec) { fundecs.push_back(fundec); };

    FunDecList() = default;
    ~FunDecList();
    int accept(Visitor* visitor);
};

class VarDec {
public:
    bool is_mut;
    string type;
    string var;
    bool is_implicit;
    AssignStatement* stm;

    VarDec(bool is_mut, string var, bool is_implicit, AssignStatement* stm);

    VarDec(bool is_mut, string type, string var, bool is_implicit, AssignStatement* stm);

    int accept(Visitor* visitor);
    ~VarDec();
};

class VarDecList {
public:
    list<VarDec*> vardecs;
    void add(VarDec* vardec);

    VarDecList() = default;
    ~VarDecList();
    int accept(Visitor* visitor);
};

class ClassDec {
public:
    string id;
    VarDecList* vdl;
    ParamList* pl;

    ClassDec(): vdl(nullptr), pl(nullptr) {
    }

    ~ClassDec();
    int accept(Visitor* visitor);
};

class ClassDecList {
public:
    list<ClassDec*> classdecs;
    void add(ClassDec* cd) { classdecs.push_back(cd); }

    ClassDecList() = default;
    ~ClassDecList();
    int accept(Visitor* visitor);
};

class Program {
public:
    VarDecList* vl;
    FunDecList* fl;
    ClassDecList* cl;
    StmtList* stml;

    Program() : vl(nullptr), fl(nullptr), cl(nullptr), stml(nullptr) {
    }

    ~Program();
    int accept(Visitor* visitor);
};

#endif
