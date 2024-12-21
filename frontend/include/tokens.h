#ifndef TOKENS_H
#define TOKENS_H

#include "syntax_tree.h"

enum class TokenType{
    CONSTANT            = 1,
    IDENTIFIER          = 2,
    KEYWORD             = 3,
};

typedef NodeValue TokenValue;

struct Token{
    TokenType           token_type;
    TokenValue          value;
    size_t              line;
    char*               lexem_pos;

    Token*              next;
};

Token* createToken(TokenType type, TokenValue value, size_t line, char* lexem_pos);

#endif
