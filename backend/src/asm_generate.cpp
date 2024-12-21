#include <stdio.h>

#include "asm_generate.h"

#include "syntax_tree.h"

static void generateVarDecl(Node* node, FILE* asm_code);
static void generateKeywords(Node* node, FILE* asm_code);
static void generateExp(Node* node, FILE* asm_code);
static void printOper(Node* node, FILE* asm_code);

void generateCode(Node* node, FILE* asm_code){
    if (node == NULL) return;

    if (_TYPE(node) == KEYWORD && _KWD(node) == SEMICOLON){
        generateCode(_L(node), asm_code);
        generateCode(_R(node), asm_code);

        return;
    }

    switch(_TYPE(node)){
        case(VAR_DECLARATION):{
            generateVarDecl(node, asm_code);
            break;
        }
        case(KEYWORD):{
            generateKeywords(node, asm_code);
            break;
        }
        default: break;
    }
}

static void generateVarDecl(Node* node, FILE* asm_code){
    if (_TYPE(_R(node)) == KEYWORD && _KWD(_R(node)) == ASSIGN){
        if (_TYPE(_L(_R(node))) && _KWD(_L(_R(node))) == IN){
            fprintf(asm_code, "IN\n");
            fprintf(asm_code, "POP [%ld]\n", _INX(node));

            return;
        }

        generateExp(_L(_R(node)), asm_code);
        fprintf(asm_code, "POP [%ld]\n", _INX(node));
    }
}

static void generateKeywords(Node* node, FILE* asm_code){
    if (_KWD(node) == OUT){
        fprintf(asm_code, "PUSH [%ld]\n", _INX(_L(node)));
        fprintf(asm_code, "OUT\n");

        return;
    }
}

static void generateExp(Node* node, FILE* asm_code){
    if (node == NULL) return;

    if (_TYPE(node) == KEYWORD){
        generateExp(_L(node), asm_code);
        generateExp(_R(node), asm_code);
        printOper(node, asm_code);
    }
    else if (_TYPE(node) == CONSTANT){
        fprintf(asm_code, "PUSH %d\n", _NUM(node));
    }
    else{
        fprintf(asm_code, "PUSH [%ld]\n", _INX(node));
    }
}

#define _DESCR(enumer) case(enumer): fprintf(asm_code, #enumer "\n");break;
static void printOper(Node* node, FILE* asm_code){
    switch(_KWD(node)){
        _DESCR(ADD);
        _DESCR(SUB);
        _DESCR(DIV);
        _DESCR(MULT);
        _DESCR(SQRT);

        default: break;
    }
}
