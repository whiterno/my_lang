#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tokens.h"
#include "lexer.h"

Token* createToken(Tkn::TokenType type, TokenValue value, size_t line, char* lexem_pos){
    Token* token = (Token*)calloc(1, sizeof(Token));

    token->token_type   = type;
    token->line         = line;
    token->lexem_pos    = lexem_pos;
    token->next         = NULL;
    memcpy(&token->value, &value, sizeof(TokenValue));

    return token;
}
