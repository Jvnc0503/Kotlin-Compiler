#ifndef DECLARATION_H
#define DECLARATION_H

#include "iostream"
using namespace std;

struct Declaration {
    string type;
    bool is_mut;
    bool is_implicit;

    Declaration(string t = "undefines", bool mut = false, bool impl = false) : type(t), is_mut(mut), is_implicit(impl) {}
};

#endif  // DECLARATION_H
