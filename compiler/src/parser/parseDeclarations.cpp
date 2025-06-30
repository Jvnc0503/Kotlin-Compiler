#include <unordered_map>
#include "parser.h"
#include "../exp/exp.h"
#include "../scanner/token.h"

VarDecList* Parser::parseVarDecList() {
    consumeENDL();
    auto* vdl = new VarDecList();
    VarDec* aux;
    while ((aux = parseVarDec())) {
        vdl->add(aux);
        consumeENDL();
    }
    return vdl;
}

VarDec* Parser::parseVarDec() {
    consumeENDL();
    if (!(check(Token::VAL) || check(Token::VAR))) {
        return nullptr;
    }
    if (!(match(Token::VAR) || match(Token::VAL))) {
        errorHandler("VAR | VAL", "VarDec");
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

static std::unordered_map<Token::Type, std::string> typeMap = {{Token::BOOL_TYPE, "Boolean"}, {Token::INT_TYPE, "Int"}, {Token::UNIT_TYPE, "Unit"}};

VarDec* Parser::handleVarDecWithExplicitType(const bool is_mut, const std::string& name) {
    if (!(match(Token::BOOL_TYPE) || match(Token::INT_TYPE) || match(Token::UNIT_TYPE))) {
        std::cerr << "Error: se esperaba un tipo válido después de ':'\n";
        exit(1);
    }
    const std::string type = typeMap[previous->type];
    AssignStatement* stm = nullptr;
    if (match(Token::ASSIGN)) {
        Exp* e = parseExpression();
        stm = new AssignStatement(name, e);
    }
    if (!(match(Token::ENDL) || match(Token::SEMICOLON))) {
        errorHandler("ENDL | SEMICOLON", "VarDec");
    }
    consumeENDL();
    return new VarDec(is_mut, name, type, false, stm);
}

FunDecList* Parser::parseFunDecList() {
    consumeENDL();
    auto* fdl = new FunDecList();

    FunDec* aux = parseFunDec();
    while (aux) {
        fdl->add(aux);
        consumeENDL();
        aux = parseFunDec();
    }
    return fdl;
}

FunDec* Parser::parseFunDec() {
    consumeENDL();
    if (!check(Token::FUN)) {
        return nullptr;
    }
    auto* fundec = new FunDec();
    if (!match(Token::FUN)) {
        errorHandler("FUN", "FUNDEC");
    }
    if (!match(Token::ID)) {
        errorHandler("ID", "FUNDEC");
    }
    std::string id = previous->text;
    if (!match(Token::LPAREN)) {
        errorHandler("LPAREN", "FUNDEC");
    }
    ParamList* paramlist = parseParamList();
    if (!match(Token::RPAREN)) {
        errorHandler("RPAREN", "FUNDEC");
    }
    if (match(Token::COLON)) {
        if (!match(Token::INT_TYPE) || match(Token::BOOL_TYPE) || match(Token::UNIT_TYPE)) {
            errorHandler("TYPE", "FUNDEC");
        }
        fundec->type = previous->text;
    } else {
        fundec->type = "Unit";
    }
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "FUNDEC");
    }
    Block* block = parseBlock();
    if (!match(Token::RBRACE)) {
        errorHandler("RBRACE", "FUNDEC");
    }
    fundec->block = block;
    fundec->paramList = paramlist;
    fundec->id = id;
    return fundec;
}

ParamList* Parser::parseParamList() {
    auto* pl = new ParamList();

    Param* param = parseParam();
    if (!param) {
        return pl;
    }
    pl->add(param);
    while (match(Token::COMMA)) {
        consumeENDL();
        param = parseParam();
        if (!param) {
            errorHandler("PARAM", "PARAMLIST");
        }
        pl->add(param);
    }
    return pl;
}

Param* Parser::parseParam() {
    if (check(Token::VAL)) {
        advance();
    }
    if (!match(Token::ID)) {
        return nullptr;
    }
    std::string id = previous->text;

    if (!match(Token::COLON)) {
        errorHandler("COLON", "PARAM");
    }

    std::string type;

    if (match(Token::INT_TYPE)) {
        type = "Int";
    } else if (match(Token::BOOL_TYPE)) {
        type = "Boolean";
    } else if (match(Token::UNIT_TYPE)) {
        type = "Unit";
    } else if (match(Token::ID)) {
        type = previous->text;
    } else {
        errorHandler("TYPE O ID", "PARAMLIST");
    }

    return new Param(id, type);
}

ClassDecList* Parser::parseClassDecList() {
    consumeENDL();
    auto* cdl = new ClassDecList();

    ClassDec* aux = parseClassDec();
    while (aux) {
        cdl->add(aux);
        consumeENDL();
        aux = parseClassDec();
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
    ParamList* pl = parseParamList();
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