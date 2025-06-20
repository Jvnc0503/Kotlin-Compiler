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
        VarDec* vd = new VarDec();
        if (match(Token::VAL)) {
            vd->is_mut = false;
        } else if (match(Token::VAR)) {
            vd->is_mut = true;
        }
        if (!match(Token::ID)) {
            errorHandler("ID", "VarDec");
        }
        vd->var = previous->text;
        string var = previous->text;
        if (!match(Token::COLON)) {
            errorHandler("':'", "VarDec");
        }
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
            AssignStatement* stm = new AssignStatement(var, e);
            vd->stm = stm;
        }
        if (!match(Token::ENDL)) {
            errorHandler("ENDL", "VarDec");
        }
        consumeENDL();
        return vd;
    }
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
    if (!match(Token::COLON)) {
        errorHandler("COLON", "FUNDEC");
    }
    if (!match(Token::INT_TYPE) || match(Token::BOOL_TYPE) || match(Token::UNIT_TYPE)) {
        errorHandler("TYPE", "FUNDEC");
    }
    string type = previous->text;
    if (!match(Token::LBRACE)) {
        errorHandler("LBRACE", "FUNDEC");
    }
    Block* block = parseBlock();
    if (!match(Token::RBRACE)) {
        cout << current->text << endl;
        errorHandler("RBRACE", "FUNDEC");
    }
    fundec->block = block;
    fundec->paramList = paramlist;
    fundec->id = id;
    fundec->type = type;
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
    if (match(Token::ID)) {
        string id = previous->text;
        if (!match(Token::COLON)) {
            errorHandler("COLON", "PARAM");
        }
        if (!match(Token::ID)) {
            errorHandler("ID", "PARAM");
        }
        string type = previous->text;
        return new Param(id, type);
    }
    return nullptr;
}