#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "parsing.h"
#include "lexer.h"
#include "nametable.h"

#include "syntax_tree.h"

static size_t scope_cnt = 0;
static size_t func_cnt  = 0;

static Node* getProgramm(Token** tokens, Nametable* nt);
static Node* getStatement(Token** tokens, Nametable* nt);

static Node* getFuncDeclaration(Token** tokens, Nametable* nt);
static Node* getVarDeclaration(Token** tokens, Nametable* nt);
static Node* getParameters(Token** tokens, Nametable* nt);
static Node* getFuncCall(Token** tokens, Nametable* nt);
static Node* getAssignment(Token** tokens, Nametable* nt);
static Node* getIf(Token** tokens, Nametable* nt);

static Node* getExpression(Token** tokens, Nametable* nt);
static Node* getPriority5(Token** tokens, Nametable* nt);
static Node* getPriority4(Token** tokens, Nametable* nt);
static Node* getPriority3(Token** tokens, Nametable* nt);
static Node* getPriority2(Token** tokens, Nametable* nt);
static Node* getPriority1(Token** tokens, Nametable* nt);
static Node* getPriority0(Token** tokens, Nametable* nt);

static Node* getValue(Token** tokens, Nametable* nt);
static Node* getIdentifier(Token** tokens, Nametable* nt);
static Node* getNumber(Token** tokens, Nametable* nt);

static Node* getArguments(Token** tokens, Nametable* nt);
static Node* getArg(Token** tokens, Nametable* nt);

static void syntaxError(SyntaxError err, size_t line);
static const char* errString(SyntaxError err);

Node* parsing (const char* filename){
    assert(filename);

    Nametable* nt = nametableCtor();
    Token* tokens = lexer(filename);

    printTokens(tokens);

    Node* root = getProgramm(&tokens->next, nt);

    printf("size: %ld\n", nt->size);
    printf("name[0]: *%s* \n", nt->names[0].name.id);

    return root;
}

static Node* getProgramm(Token** tokens, Nametable* nt){
    if (*tokens == NULL || (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACE_CL)) return NULL;

    Node* statement_node = getStatement(tokens, nt);

    return _SEMICOLON(statement_node);
}

static Node* getStatement(Token** tokens, Nametable* nt){
    if (*tokens == NULL) return NULL;

    Node* statement_node = NULL;
    if ((statement_node = getFuncDeclaration(tokens, nt)))      return statement_node;
    if ((statement_node = getVarDeclaration(tokens, nt)))       return statement_node;
    if ((statement_node = getFuncCall(tokens, nt)) &&
        _TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == SEMICOLON){
            *tokens = _NT;

            return statement_node;
        }
    if ((statement_node = getAssignment(tokens, nt)))            return statement_node;
    if ((statement_node = getIf(tokens, nt)))                    return statement_node;

    return NULL;
}

static Node* getFuncDeclaration(Token** tokens, Nametable* nt){
    if (_T == NULL || _NT == NULL || _NNT == NULL) return NULL;

    _CHECK_FUNC_DECL_TEMP(return NULL);
    if (isInit(nt, _IDENT_T(_NT))) syntaxError(DOUBLE_INITIALIZATION, _LINE(_T));

    func_cnt++;

    if (isNameInNametable(nt, _IDENT_T(_NT)))   initName(nt, _IDENT_T(_NT));
    else                                        addNewName(nt, _IDENT_T(_NT), _LINE(_NT), true);

    Token* func_ident = _NT;
    *tokens = _NNT;

    Node* params_tree       = getParameters(tokens, nt);
    _CHECK_CLOSE_BRACKET(syntaxError(WRONG_FUNC_DECLARATION, _LINE(_T)));
    *tokens = _NT;
    _CHECK_OPEN_BRACE(syntaxError(WRONG_FUNC_DECLARATION, _LINE(_T)));
    *tokens = _NT;

    Node* func_body         = getProgramm(tokens, nt);
    Node* parameters_node   = createNode(params_tree, func_body, PARAMETERS, DUMMY);


    _CHECK_CLOSE_BRACE(syntaxError(WRONG_FUNC_DECLARATION, _LINE(_T)));
    *tokens = _NT;
    func_cnt--;

    return _FUNC_DEF(func_ident);
}

static Node* getParameters(Token** tokens, Nametable* nt){
    if (_NT == NULL || _NNT == NULL) syntaxError(WRONG_FUNC_DECLARATION, _LINE(_T));

    if (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACKET_CL) return NULL;
    Node* comma = createNode(NULL, NULL, KEYWORD, (NodeValue){.keyword_type = COMMA});

    *tokens = _NT;

    _CHECK_VAR_DECL_TEMP(syntaxError(WRONG_VAR_DECLARATION, _LINE(_T)));
    if (isInit(nt, _IDENT_T(_NT))) syntaxError(DOUBLE_INITIALIZATION, _LINE(_T));

    if (isNameInNametable(nt, _IDENT_T(_NT)))   syntaxError(DOUBLE_INITIALIZATION, _LINE(_T));
    else                                        addNewName(nt, _IDENT_T(_NT), _LINE(_NT), true);

    Node* var_node  = _VAR_DECL(_NT)
    _R(var_node)    = _IDENT_N(_NT);
    *tokens         = _NNT;

    _L(comma)       = getParameters(tokens, nt);
    _R(comma)       = var_node;

    return comma;
}

static Node* getVarDeclaration(Token** tokens, Nametable* nt){
    if (_T == NULL || _NT == NULL || _NNT == NULL) return NULL;

    _CHECK_VAR_DECL_TEMP(return NULL);
    if (isInit(nt, _IDENT_T(_NT))) syntaxError(DOUBLE_INITIALIZATION, _LINE(_T));

    if (isNameInNametable(nt, _IDENT_T(_NT)))   syntaxError(DOUBLE_INITIALIZATION, _LINE(_T));
    else                                        addNewName(nt, _IDENT_T(_NT), _LINE(_NT), true);

    Node* ident_node    = _IDENT_N(_NT);
    Node* var_decl_node = _VAR_DECL(_NT)

    Node* var_ident     = _IDENT_N(_NT);
    *tokens             = _NNT;

    if (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == ASSIGN){
        *tokens = _NT;
        _R(var_decl_node)       = _KWD_N(ASSIGN);
        _R(_R(var_decl_node))   = var_ident;
        _L(_R(var_decl_node))   = getExpression(tokens, nt);
    }
    else{
        _R(var_decl_node)       = var_ident;
    }

    _CHECK_SEMICOLON(syntaxError(NO_SEMICOLON, _LINE(_T)));
    *tokens = _NT;

    return var_decl_node;
}

static Node* getAssignment(Token** tokens, Nametable* nt){
    _CHECK_ASIGN_TEMP(return NULL);

    if (! (isNameInNametable(nt, _IDENT_T(_T)))) syntaxError(NO_IDENT_IN_NAMETABLE, _LINE(_T));

    Node* assign    = _KWD_N(ASSIGN);
    _L(assign)      = _IDENT_N(_T);
    *tokens         = _NNT;
    _R(assign)      = getExpression(tokens, nt);

    _CHECK_SEMICOLON(syntaxError(NO_SEMICOLON, _LINE(_T)));
    *tokens         = _NT;

    return assign;
}

static Node* getIf(Token** tokens, Nametable* nt){
    _CHECK_IF_TEMP(return NULL);

    Node* if_node = _KWD_N(IF);
    *tokens = _NNT;

    _L(if_node) = getExpression(tokens, nt);

    _CHECK_CLOSE_BRACKET(syntaxError(WRONG_IF, _LINE(_T)));
    *tokens     = _NT;
    _CHECK_OPEN_BRACE(syntaxError(WRONG_IF, _LINE(_T)));
    *tokens     = _NT;

    _R(if_node) = getProgramm(tokens, nt);
    _CHECK_CLOSE_BRACE(syntaxError(WRONG_IF, _LINE(_T)));
    *tokens     = _NT;

    return if_node;
}

static Node* getExpression(Token** tokens, Nametable* nt){
    if (_T == NULL) return NULL;

    return getPriority5(tokens, nt);
}

static Node* getPriority5(Token** tokens, Nametable* nt){
    if (_T == NULL) return NULL;

    Node* priority4_node = getPriority4(tokens, nt);

    _PRIORITIES(AND, 4, 5);
    _PRIORITIES(OR, 4, 5);

    return priority4_node;
}
static Node* getPriority4(Token** tokens, Nametable* nt){
    Node* priority3_node = getPriority3(tokens, nt);

    _PRIORITIES(EQUAL, 3, 4);
    _PRIORITIES(LESS, 3, 4);
    _PRIORITIES(GREATER, 3, 4);
    _PRIORITIES(L_OR_EQ, 3, 4);
    _PRIORITIES(G_OR_EQ, 3, 4);
    _PRIORITIES(N_EQ, 3, 4);

    return priority3_node;
}

static Node* getPriority3(Token** tokens, Nametable* nt){
    Node* priority2_node = getPriority2(tokens, nt);
    size_t sub_cnt = 0;
    size_t add_cnt = 0;

    while (_TYPE_T(_T) == TokenType::KEYWORD && (_KWD_T(_T) == SUB || _KWD_T(_T) == ADD)){
        if (_KWD_T(_T) == SUB) sub_cnt++;
        else                   add_cnt++;
        *tokens = _NT;
    }

    if (sub_cnt % 2 == 0 && sub_cnt + add_cnt != 0) return _ADD(priority2_node, getPriority3(tokens, nt));
    if (sub_cnt % 2 == 1)                           return _SUB(priority2_node, getPriority3(tokens, nt));

    return priority2_node;
}

static Node* getPriority2(Token** tokens, Nametable* nt){
    Node* priority1_node = getPriority1(tokens, nt);

    _PRIORITIES(MULT, 1, 2);
    _PRIORITIES(DIV, 1, 2);

    return priority1_node;
}

static Node* getPriority1(Token** tokens, Nametable* nt){
    Node* priority0_node = getPriority0(tokens, nt);

    _PRIORITIES(POW, 0, 1);

    return priority0_node;
}

static Node* getPriority0(Token** tokens, Nametable* nt){
    if (_TYPE_T(_T) == TokenType::KEYWORD){
        if (_KWD_T(_T) == SIN){
            _PRIORITY0_FUNC(SIN);
        }
        if (_KWD_T(_T) == COS){
            _PRIORITY0_FUNC(COS);
        }
        if (_KWD_T(_T) == SQRT){
            _PRIORITY0_FUNC(SQRT);
        }
        if (_KWD_T(_T) == NOT){
            *tokens = _NT;
            return _NOT(getValue(tokens, nt));
        }
    }

    return getValue(tokens, nt);
}

static Node* getValue(Token** tokens, Nametable* nt){
    if (_TYPE_T(_T) == TokenType::KEYWORD){
        _CHECK_OPEN_BRACKET(syntaxError(EXPRESSION_NOT_FULL, _LINE(_T)));

        if (_TYPE_T(_NT) == TokenType::KEYWORD && _KWD_T(_NT) == SUB){
            *tokens = _NNT;
            Node* ret = getNumber(tokens, nt);
            *tokens = _NT;

            ret->value.number = (-1) * ret->value.number;

            return ret;
        }

        *tokens = _NT;
        Node* ret = getPriority5(tokens, nt);

        _CHECK_CLOSE_BRACKET(syntaxError(EXPRESSION_NOT_FULL, _LINE(_T)));
        *tokens = _NT;

        return ret;
    }
    if (_TYPE_T(_T) == TokenType::CONSTANT){
        return getNumber(tokens, nt);
    }
    if (_TYPE_T(_T) == TokenType::IDENTIFIER){
        if (_TYPE_T(_NT) == TokenType::KEYWORD && _KWD_T(_NT) == BRACE_OP){
            Node* ret = getFuncCall(tokens, nt);
            if (ret == NULL) syntaxError(WRONG_FUNC_CALL, _LINE(_T));

            return ret;
        }

        return getIdentifier(tokens, nt);
    }

    return NULL;
}

static Node* getIdentifier(Token** tokens, Nametable* nt){
    Node* ident = _IDENT_N(_T);
    *tokens = _NT;

    return ident;
}

static Node* getNumber(Token** tokens, Nametable* nt){
    Node* num = _CONST(_T);
    *tokens = _NT;

    return num;
}

static Node* getFuncCall(Token** tokens, Nametable* nt){
    _CHECK_FUNC_DECL_TEMP(return NULL);

    if (!(isNameInNametable(nt, _IDENT_T(_T)))) addNewName(nt, _IDENT_T(_T), _LINE(_T), false);

    Token* ident_token  = _T;
    *tokens             = _NT;
    Node* func_call     = _FUNC_CALL(ident_token);

    return func_call;
}

static Node* getArguments(Token** tokens, Nametable* nt){
    if (_TYPE_T(_NT) == TokenType::KEYWORD && _KWD_T(_NT) == BRACKET_OP){
        *tokens = _NNT;

        return NULL;
    }
    if (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACKET_CL){
        *tokens = _NT;

        return NULL;
    }

    if (!(_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == COMMA)) syntaxError(WRONG_FUNC_CALL, _LINE(_T));

    Node* comma = createNode(NULL, NULL, KEYWORD, (NodeValue){.keyword_type = COMMA});

    if (!(isNameInNametable(nt, _IDENT_T(_NT)))) syntaxError(NO_IDENT_IN_NAMETABLE, _LINE(_T));

    *tokens     = _NT;
    Node* arg   = getArg(tokens, nt);
    _R(comma)   = arg;
    _L(comma)   = getArguments(tokens, nt);

    return comma;
}

static Node* getArg(Token** tokens, Nametable* nt){
    if (_TYPE_T(_T) == TokenType::CONSTANT) return getNumber(tokens, nt);

    return getIdentifier(tokens, nt);
}

static void syntaxError(SyntaxError err, size_t line){
    printf("\033[31;1;4mLINE %ld:ERROR:\033[0m %s\n", line, errString(err));

    abort();
}

#define _DESCR(err) case err: return #err;
static const char* errString(SyntaxError err){
    switch(err){
        _DESCR(DOUBLE_INITIALIZATION);
        _DESCR(WRONG_VAR_DECLARATION);
        _DESCR(WRONG_FUNC_DECLARATION);
        _DESCR(NO_SEMICOLON);
        _DESCR(NO_IDENT_IN_NAMETABLE);
        _DESCR(NO_OPENING_BRACKET);
        _DESCR(NO_CLOSING_BRACKET);
        _DESCR(EXPRESSION_NOT_FULL);
        _DESCR(WRONG_FUNC_CALL);
        _DESCR(WRONG_PROGRAMM);
        _DESCR(WRONG_IF);
    }
}
