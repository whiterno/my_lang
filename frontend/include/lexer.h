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

#define _NUM(value)          createToken(TokenType::CONSTANT, (TokenValue){.number = value}, 0, NULL)
#define _ID_TOK(value)       createToken(TokenType::IDENTIFIER, (TokenValue){.name = _ID(value)}, 0, NULL)
#define _KEYWORD(index)      createToken(TokenType::KEYWORD, (TokenValue){.keyword_type = index}, 0, NULL)

#define RED                  "\033[33;1;4m"
#define GREEN                "\033[33;1;4m"
#define BLUE                 "\033[34;1;4m"
#define MAGENTA              "\033[35;1;4m"
#define STOP                 "\033[0m"

Token* lexer(const char* filename);
void printTokens(Token* head);

#endif
