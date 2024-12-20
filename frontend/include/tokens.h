#ifndef TOKENS_H
#define TOKENS_H

#include "syntax_tree.h"

namespace Tkn{
    enum TokenType{
        CONSTANT            = 1,
        IDENTIFIER          = 2,
        KEYWORD             = 3,
    };
}

typedef NodeValue TokenValue;

struct Token{
    Tkn::TokenType      token_type;
    TokenValue          value;
    size_t              line;
    char*               lexem_pos;

    Token*              next;
};

Token* createToken(Tkn::TokenType type, TokenValue value, size_t line, char* lexem_pos);

#endif
