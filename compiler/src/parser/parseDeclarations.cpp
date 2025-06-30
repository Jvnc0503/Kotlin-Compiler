#include <unordered_map>
#include "parser.h"
#include "../exp/exp.h"
#include "../scanner/token.h"

VarDecList* Parser::parseVarDecList() {
    consumeENDL();
    auto* vdl = new VarDecList();
    VarDec* aux = nullptr;
    while ((aux = parseVarDec())) {
        vdl->add(aux);
        if (!(match(Token::ENDL) || match(Token::SEMICOLON))) {
            errorHandler("ENDL | SEMICOLON", "VarDec");
        }
        consumeSeparator();
    }
    return vdl;
}

VarDec* Parser::parseVarDec() {
    consumeENDL();
    if (!(match(Token::VAL) || match(Token::VAR))) {
        return nullptr;
    }
    const bool is_mut = previous->type == Token::VAR;
    if (!match(Token::ID)) {
        errorHandler("ID", "VarDec");
    }
    const std::string name = previous->text;
    if (match(Token::COLON)) {
        return handleVarDecWithExplicitType(is_mut, name);
    }
    return handleVarDecWithImplicitType(is_mut, name);
}

VarDec* Parser::handleVarDecWithImplicitType(const bool is_mut, const std::string& name) {
    if (!match(Token::ASSIGN)) {
        errorHandler("ASSIGN", "VarDec");
    }
    Exp* e = parseExpression();
    auto* stm = new AssignStatement(name, e);
    return new VarDec(is_mut, name, true, stm);
}

VarDec* Parser::handleVarDecWithExplicitType(const bool is_mut, const std::string& name) {
    if (!(match(Token::BOOL_TYPE) || match(Token::INT_TYPE) || match(Token::UNIT_TYPE) || match(Token::ID))) {
        std::cerr << "Error: se esperaba un tipo después de ':'\n";
        exit(1);
    }
    const std::string type = previous->text;
    AssignStatement* stm = nullptr;
    if (match(Token::ASSIGN)) {
        Exp* e = parseExpression();
        stm = new AssignStatement(name, e);
    }
    return new VarDec(is_mut, name, type, false, stm);
}

FunDecList* Parser::parseFunDecList() {
    consumeENDL();
    auto* fdl = new FunDecList();
    FunDec* aux = nullptr;
    while ((aux = parseFunDec())) {
        fdl->add(aux);
        consumeSeparator();
    }
    return fdl;
}

FunDec* Parser::parseFunDec() {
    consumeENDL();
    if (!check(Token::FUN)) {
        return nullptr;
    }
    if (!match(Token::FUN)) {
        errorHandler("FUN", "FUNDEC");
    }
    if (!match(Token::ID)) {
        errorHandler("ID", "FUNDEC");
    }
    const std::string id = previous->text;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "FUNDEC");
    }
    ParamList* paramlist = parseFunParamList();
    if (!match(Token::RPAREN)) {
        std::cout << current;
        errorHandler("RPAREN", "FUNDEC");
    }
    std::string type;
    if (match(Token::COLON)) {
        if (!match(Token::INT_TYPE) || match(Token::BOOL_TYPE) || match(Token::UNIT_TYPE) || match(Token::ID)) {
            errorHandler("TYPE", "FUNDEC");
        }
        type = previous->text;
    } else {
        type = "Unit";
    }
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "FUNDEC");
    }
    Block* block = parseBlock();
    if (!match(Token::RBRACE)) {
        errorHandler("RBRACE", "FUNDEC");
    }
    return new FunDec(id, type, paramlist, block);
}

ParamList* Parser::parseFunParamList() {
    auto* pl = new ParamList();

    Param* param = parseFunParam();
    if (param == nullptr) {
        return pl;
    }
    pl->add(param);
    while (match(Token::COMMA)) {
        consumeENDL();
        param = parseFunParam();
        if (!param) {
            errorHandler("PARAM", "PARAMLIST");
        }
        pl->add(param);
    }
    return pl;
}

Param* Parser::parseFunParam() {
    if (!match(Token::ID)) {
        return nullptr;
    }
    const std::string id = previous->text;
    if (!match(Token::COLON)) {
        errorHandler("COLON", "PARAM");
    }
    if (!(match(Token::BOOL_TYPE) || match(Token::INT_TYPE) || match(Token::UNIT_TYPE) || match(Token::ID))) {
        std::cerr << "Error: se esperaba un tipo válido después de ':'\n";
        exit(1);
    }
    const std::string type = previous->text;
    return new Param(id, type);
}

ParamList* Parser::parseClassParamList() {
    auto* pl = new ParamList();

    Param* param = parseClassParam();
    if (param == nullptr) {
        return pl;
    }
    pl->add(param);
    while (match(Token::COMMA)) {
        consumeENDL();
        param = parseClassParam();
        if (!param) {
            errorHandler("PARAM", "PARAMLIST");
        }
        pl->add(param);
    }
    return pl;
}

Param* Parser::parseClassParam() {
    if (!match(Token::VAL)) {
        return nullptr;
    }
    if (!match(Token::ID)) {
        errorHandler("ID", "PARAM");
    }
    const std::string id = previous->text;
    if (!match(Token::COLON)) {
        errorHandler("COLON", "PARAM");
    }
    if (!(match(Token::BOOL_TYPE) || match(Token::INT_TYPE) || match(Token::UNIT_TYPE) || match(Token::ID))) {
        std::cerr << "Error: se esperaba un tipo válido después de ':'\n";
        exit(1);
    }
    const std::string type = previous->text;
    return new Param(id, type);
}

ClassDecList* Parser::parseClassDecList() {
    consumeENDL();
    auto* cdl = new ClassDecList();
    ClassDec* aux = nullptr;
    while ((aux = parseClassDec())) {
        cdl->add(aux);
        consumeSeparator();
    }
    return cdl;
}

ClassDec* Parser::parseClassDec() {
    consumeENDL();
    if (!match(Token::CLASS)) {
        return nullptr;
    }
    auto* cd = new ClassDec();
    if (!match(Token::ID)) {
        errorHandler("ID", "CLASSDEC");
    }
    cd->id = previous->text;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "CLASSDECS");
    }
    ParamList* pl = parseClassParamList();
    if (!match(Token::RPAREN)) {
        errorHandler("RPAREN", "CLASSDECS");
    }
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "CLASSDEC");
    }
    VarDecList* vdl = parseVarDecList();
    if (!match(Token::RBRACE)) {
        errorHandler("RBRACE", "CLASSDEC");
    }
    cd->pl = pl;
    cd->vdl = vdl;
    return cd;
}