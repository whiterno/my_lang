#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"

const int MAX_LEXEM_LENGTH = 128;

const char WHILE_STR[]          = "while";
const char IF_STR[]             = "if";
const char DECLARATION_STR[]    = "int";
const char BREAK_STR[]          = "break";
const char CONTINUE_STR[]       = "continue";
const char RETURN_STR[]         = "return";
const char IN_STR[]             = "input";
const char OUT_STR[]            = "print";

#define _ID(value)           createIdentifier(value)

#define _NUM(value)          createToken(Tkn::CONSTANT, (TokenValue){.number = value}, 0, NULL)
#define _ID_TOK(value)       createToken(Tkn::IDENTIFIER, (TokenValue){.name = _ID(value)}, 0, NULL)
#define _KEYWORD(index)      createToken(Tkn::KEYWORD, (TokenValue){.keyword_type = index}, 0, NULL)

Token* lexer(const char* filename);
void printTokens(Token* head);

#endif
