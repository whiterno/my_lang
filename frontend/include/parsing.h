#ifndef PARSING_H
#define PARSING_H

#define DUMMY                       (NodeValue){.number = 0}

#define _T                          (*tokens)
#define _NT                         (*tokens)->next
#define _NNT                        (*tokens)->next->next

#define _L(node)                    (node)->left
#define _R(node)                    (node)->right

#define _TYPE_T(token)              token->token_type
#define _KWD_T(token)               token->value.keyword_type
#define _NUM_T(token)               token->value.number
#define _IDENT_T(token)             token->value.name

#define _CHECK_FUNC_DECL_TEMP(...)       if (! (_TYPE_T(_T) == Tkn::KEYWORD && _KWD_T(_T) == DECLARATION &&                  \
                                               _TYPE_T(_NT) == Tkn::IDENTIFIER &&                                           \
                                               _TYPE_T(_NNT) == Tkn::KEYWORD && _KWD_T(_NNT) == BRACKET_OP)) __VA_ARGS__;
#define _CHECK_VAR_DECL_TEMP(...)        if (! (_KWD_T(_T) == DECLARATION && _TYPE_T(_NT) == Tkn::IDENTIFIER)) __VA_ARGS__;
#define _CHECK_SEMICOLON(...)            if (! (_TYPE_T(_T) == KEYWORD && _KWD_T(_T) == SEMICOLON)) __VA_ARGS__;

#define _CONST(token)               createNode(NULL, NULL, CONSTANT, (NodeValue){.number = token->value.number})
#define _IDENT_N(token)             createNode(NULL, NULL, IDENTIFIER, (NodeValue){.index = getIndex(nt, _IDENT_T(token))})
#define _KWD_N(kwd_type)            createNode(NULL, NULL, KEYWORD, (NodeValue){.keyword_type = kwd_type})
#define _VAR_DECL(token)            createNode(NULL, NULL, VAR_DECLARATION, (NodeValue){.index = getIndex(nt, _IDENT_T(token))});
#define _FUNC_DEF(token)            createNode(_KWD_N(NUMBER), parameters_node, FUNCTION_DEFINITION, (NodeValue){.index = getIndex(nt, _IDENT_T(token))})

#define _SEMICOLON(statement)       createNode(statement, getProgramm(tokens, nt), KEYWORD, (NodeValue){.keyword_type = SEMICOLON})

enum SyntaxError{
    DOUBLE_INITIALIZATION   = 1,
    WRONG_FUNC_DECLARATION  = 2,
    WRONG_VAR_DECLARATION   = 3,
    NO_SEMICOLON            = 4,
    NO_IDENT_IN_NAMETABLE   = 5,
    NO_OPENING_BRACKET      = 6,
    NO_CLOSING_BRACKET      = 7,
};

#endif
