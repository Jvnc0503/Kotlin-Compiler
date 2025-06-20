#include "parser.h"

VarDecList* Parser::parseVarDecList() {
    consumeENDL();
    VarDecList* vdl = new VarDecList();
    VarDec* aux = parseVarDec();
    while (aux) {
        vdl->add(aux);
        consumeENDL();
        aux = parseVarDec();
    }
    return vdl;
}

VarDec* Parser::parseVarDec() {
    consumeENDL();
    if (!(check(Token::VAL) || check(Token::VAR))) {
        return nullptr;
    } else {
        bool is_mut;
        string name;
        if (match(Token::VAL)) {
            is_mut = false;
        } else if (match(Token::VAR)) {
            is_mut = true;
        }
        if (!match(Token::ID)) {
            errorHandler("ID", "VarDec");
        }
        name = previous->text;
        if (match(Token::COLON)) {
            return handleVarDecWithExplicitType(is_mut, name);
        } else {
            return handleVarDecWithImplicitType(is_mut, name);
        }
    }
}

VarDec* Parser::handleVarDecWithImplicitType(bool is_mut, string name) {
    VarDec* vd = new VarDec();
    vd->is_mut = is_mut;
    vd->var = name;
    vd->is_implicit = true;
    if (!match(Token::ASSIGN)) {
        errorHandler("ASSIGN", "VARDEC");
    }
    Exp* e = parseExpression();
    AssignStatement* astm = new AssignStatement(name, e);
    vd->stm = astm;
    return vd;
}

VarDec* Parser::handleVarDecWithExplicitType(bool is_mut, string name) {
    VarDec* vd = new VarDec();
    vd->is_mut = is_mut;
    vd->var = name;
    vd->is_implicit = false;
    if (match(Token::BOOL_TYPE)) {
        vd->type = "Boolean";
    } else if (match(Token::INT_TYPE)) {
        vd->type = "Int";
    } else if (match(Token::UNIT_TYPE)) {
        vd->type = "Unit";
    } else {
        cout << "Error: se esperaba un tipo válido después de ':'" << endl;
        exit(1);
    }
    if (match(Token::ASSIGN)) {
        Exp* e = parseExpression();
        AssignStatement* stm = new AssignStatement(name, e);
        vd->stm = stm;
    }
    if (!match(Token::ENDL)) {
        errorHandler("ENDL", "VarDec");
    }
    consumeENDL();
    return vd;
}

FunDecList* Parser::parseFunDecList() {
    consumeENDL();
    FunDecList* fdl = new FunDecList();

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
    FunDec* fundec = new FunDec();
    if (!match(Token::FUN)) {
        errorHandler("FUN", "FUNDEC");
    }
    if (!match(Token::ID)) {
        errorHandler("ID", "FUNDEC");
    }
    string id = previous->text;
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
        fundec->type = "unit";
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
    ParamList* pl = new ParamList();

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
    } else {
        errorHandler("TYPE", "PARAM");
    }

    return new Param(id, type);
}