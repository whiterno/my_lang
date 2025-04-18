#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "tokens.h"
#include "utils.h"

#include "syntax_tree.h"

static Token* getLexem(char** prg_txt);

static Token* getSymbolKeyword(char** prg_txt, char value[]);
static Token* getNumber(char** prg_txt, char value[]);
static Token* getKeyword(char** prg_txt, char value[]);

static bool isSymbol(char chr);
static Keyword getKeywordType(char value[]);

static void printKeywords(Token* token);

Token* lexer(const char* filename){
    assert(filename);

    char* prg_txt = NULL;
    readFile(filename, &prg_txt);
    char* prg_txt_beg = prg_txt;

    Token* prg_tokens   = _KEYWORD(ABORT);
    Token* head         = prg_tokens;
    size_t line         = 1;

    while (*prg_txt != '\0'){
        skipSpaces(&prg_txt, &line);
        if (*prg_txt == '\0') break;
        char* lexem_begin = prg_txt;

        head->next              = getLexem(&prg_txt);
        head->next->line        = line;
        head->next->lexem_pos   = lexem_begin;
        head                    = head->next;
    }

    printTokens(prg_tokens);

    free(prg_txt_beg);
    return prg_tokens;
}

static Token* getLexem(char** prg_txt){
    char value[MAX_LEXEM_LENGTH] = {};
    Token* token = NULL;

    if ((token = getSymbolKeyword(prg_txt, value)) != NULL)     return token;
    if ((token = getNumber(prg_txt, value)) != NULL)            return token;
    if ((token = getKeyword(prg_txt, value)) != NULL)           return token;

    return NULL;
}

static Token* getSymbolKeyword(char** prg_txt, char value[]){
    Keyword symbol_type = ABORT;

    if (isSymbol(**prg_txt)){
            value[0] = **prg_txt;
            (*prg_txt)++;
            value[1] = **prg_txt;
            value[2] = '\0';
            (*prg_txt)++;

            Keyword kwd_type = getKeywordType(value);

            if (kwd_type == NUMBER){
                value[1] = '\0';
                (*prg_txt)--;
            }
            else return _KEYWORD(kwd_type);

            return _KEYWORD(getKeywordType(value));
        }

    // if '.' return syntax error

    return NULL;
}

static Token* getNumber(char** prg_txt, char value[]){
    if (isdigit(**prg_txt)){
        for (size_t i = 0; i < MAX_LEXEM_LENGTH - 1; i++){
            if (isdigit(**prg_txt)){
                value[i] = **prg_txt;
                (*prg_txt)++;
                continue;
            }
            value[i] = '\0';
        }

        return _NUM(atoi(value));
    }

    return NULL;
}

static Token* getKeyword(char** prg_txt, char value[]){
    for (size_t i = 0; i < MAX_LEXEM_LENGTH; i++){
        if (isSymbol(**prg_txt) || isspace(**prg_txt)){
            value[i] = '\0';
            Keyword keyword_type = getKeywordType(value);

            if (keyword_type != NUMBER) return _KEYWORD(keyword_type);
            else                        return _ID_TOK(value);
        }

        value[i] = **prg_txt;
        (*prg_txt)++;
    }

    // return syntax error;

    return NULL;
}

static bool isSymbol(char chr){
    if (chr == '(' || chr == ')' ||
        chr == '{' || chr == '}' ||
        chr == '+' || chr == '-' ||
        chr == '*' || chr == '/' ||
        chr == ',' || chr == ';' ||
        chr == '>' || chr == '<' ||
        chr == '=' || chr == '!' ||
        chr == '&' || chr == '|'    ) return true;

    return false;
}

#define _DEF_KWD(str, enumer, ...)   if (strcmp(str, value) == 0) return enumer;
static Keyword getKeywordType(char value[]){

    #include "code_gen.h"

    return NUMBER;
}
#undef _DEF_KWD

// DEBUG FUNCTION

void printTokens(Token* head){
    head = head->next;

    while (head != NULL){
        switch (head->token_type){
            case TokenType::CONSTANT:      printf(MAGENTA "CONSTANT" STOP ": %d, LINE: %ld\n", head->value.number, head->line); break;
            case TokenType::IDENTIFIER:    printf(GREEN "IDENTIFIER" STOP ": %s, LINE: %ld\n", head->value.name.id, head->line); break;
            case TokenType::KEYWORD:       printKeywords(head); break;
        }
        head = head->next;
    }
}

void printToken(Token* token){
    if (token != NULL){
        switch (token->token_type){
            case TokenType::CONSTANT:      printf(MAGENTA "CONSTANT" STOP ": %d, LINE: %ld\n", token->value.number, token->line); break;
            case TokenType::IDENTIFIER:    printf(GREEN "IDENTIFIER" STOP ": %s, LINE: %ld\n", token->value.name.id, token->line); break;
            case TokenType::KEYWORD:       printKeywords(token); break;
        }
    }
}

#define _DEF_KWD(str, enum, ...) case enum: printf(BLUE "KEYWORD" STOP ": %s, LINE: %ld\n", str, token->line); break;
static void printKeywords(Token* token){
    switch (token->value.keyword_type){

        #include "code_gen.h"

        default: printf("%d?\n", token->value.keyword_type);
    }
}
#undef _DEF_KWD
