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
static Node* getFuncDeclaration(Token** tokens, Nametable* nt);
static Node* getVarDeclaration(Token** tokens, Nametable* nt);
static Node* getExpression(Token** tokens, Nametable* nt);
static Node* getParameters(Token** tokens, Nametable* nt);

static void syntaxError(SyntaxError err);
static const char* errString(SyntaxError err);

Node* parsing (const char* filename){
    assert(filename);

    Nametable* nt = nametableCtor();
    Token* tokens = lexer(filename);

    Node* root = getProgramm(&tokens->next, nt);
}

static Node* getProgramm(Token** tokens, Nametable* nt){
    if (*tokens == NULL) return NULL;
    if (_TYPE_T(_T) == KEYWORD && _KWD_T(_T) == BRACE_CL) return NULL;

    Node* statement_node = NULL;
    if ((statement_node = getFuncDeclaration(tokens, nt)) != NULL)  return _SEMICOLON(statement_node);
    if ((statement_node = getVarDeclaration(tokens, nt)) != NULL)   return _SEMICOLON(statement_node);
}

static Node* getFuncDeclaration(Token** tokens, Nametable* nt){
    if (_T == NULL || _NT == NULL || _NNT == NULL) return NULL;

    _CHECK_FUNC_DECL_TEMP(return NULL);
    if (isInit(nt, _IDENT_T(_NT))) syntaxError(DOUBLE_INITIALIZATION);

    func_cnt++;

    if (isNameInNametable(nt, _IDENT_T(_NT)))   initName(nt, _IDENT_T(_NT));
    else                                        addNewName(nt, _IDENT_T(_NT), true);

    Token* func_ident = _NT;
    *tokens = _NNT;

    Node* params_tree       = getParameters(tokens, nt);
    *tokens = _NNT;
    Node* func_body         = getProgramm(tokens, nt);
    Node* parameters_node   = createNode(params_tree, func_body, PARAMETERS, DUMMY);

    func_cnt--;

    return _FUNC_DEF(func_ident);
}

static Node* getParameters(Token** tokens, Nametable* nt){
    if (_NT == NULL || _NNT == NULL) syntaxError(WRONG_FUNC_DECLARATION);

    *tokens     = _NT;
    if (_TYPE_T(_T) == KEYWORD && _KWD_T(_T) == BRACKET_CL) return NULL;
    Node* comma = createNode(NULL, NULL, KEYWORD, (NodeValue){.keyword_type = COMMA});

    _CHECK_VAR_DECL_TEMP(syntaxError(WRONG_VAR_DECLARATION));
    if (isInit(nt, _IDENT_T(_NT))) syntaxError(DOUBLE_INITIALIZATION);

    if (isNameInNametable(nt, _IDENT_T(_NT)))   syntaxError(DOUBLE_INITIALIZATION);
    else                                        addNewName(nt, _IDENT_T(_NT), true);

    Node* var_node  = _VAR_DECL(_NT)
    *tokens         = _NNT;
    _L(comma)       = getParameters(tokens, nt);
    _R(comma)       = var_node;

    return comma;
}

static Node* getVarDeclaration(Token** tokens, Nametable* nt){
    if (_T == NULL || _NT == NULL || _NNT == NULL) return NULL;

    _CHECK_VAR_DECL_TEMP(return NULL);
    if (isInit(nt, _IDENT_T(_NT))) syntaxError(DOUBLE_INITIALIZATION);

    if (isNameInNametable(nt, _IDENT_T(_NT)))   syntaxError(DOUBLE_INITIALIZATION);
    else                                        addNewName(nt, _IDENT_T(_NT), true);

    Node* ident_node    = _IDENT_N(_NT);
    Node* var_decl_node = _VAR_DECL(_NT)
    _L(var_decl_node) = _KWD_N(NUMBER);

    Node* var_ident    = _IDENT_N(_NT);
    *tokens             = _NNT;

    if (_TYPE_T(_T) == KEYWORD && _KWD_T(_T) == ASSIGN){
        *tokens = _NT;
        _R(var_decl_node)       = _KWD_N(ASSIGN);
        _R(_R(var_decl_node))   = var_ident;
        _L(_R(var_decl_node))   = getExpression(tokens, nt);
    }
    else{
        *tokens = _NT;
        _R(var_decl_node)       = var_ident;
    }

    _CHECK_SEMICOLON(syntaxError(NO_SEMICOLON));

    return var_decl_node;
}

static Node* getExpression(Token** tokens, Nametable* nt){

}


static void syntaxError(SyntaxError err){
    printf("\033[31;1;4mERROR:\033[0m %s\n", err);
}

#define _DESCR(err) case err: return #err;
static const char* errString(SyntaxError err){
    switch(err){
        _DESCR(DOUBLE_INITIALIZATION);
        _DESCR(WRONG_VAR_DECLARATION);
        _DESCR(WRONG_FUNC_DECLARATION);
    }

    abort();
}
