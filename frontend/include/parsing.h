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
#define _LINE(token)                token->line

#define _CHECK_FUNC_DECL_TEMP(...)       if (! (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == DECLARATION &&                  \
                                               _TYPE_T(_NT) == TokenType::IDENTIFIER &&                                            \
                                               _TYPE_T(_NNT) == TokenType::KEYWORD && _KWD_T(_NNT) == BRACKET_OP)) __VA_ARGS__;
#define _CHECK_VAR_DECL_TEMP(...)        if (! (_KWD_T(_T) == DECLARATION && _TYPE_T(_NT) == TokenType::IDENTIFIER)) __VA_ARGS__;
#define _CHECK_FUNCCALL(...)
#define _CHECK_SEMICOLON(...)            if (! (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == SEMICOLON)) __VA_ARGS__;
#define _CHECK_OPEN_BRACKET(...)         if (!(_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACKET_OP)) __VA_ARGS__;
#define _CHECK_CLOSE_BRACKET(...)        if (!(_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACKET_CL)) __VA_ARGS__;
#define _CHECK_OPEN_BRACE(...)           if (!(_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACE_OP)) __VA_ARGS__;
#define _CHECK_CLOSE_BRACE(...)          if (!(_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == BRACE_CL)) __VA_ARGS__;

#define _CONST(token)               createNode(NULL, NULL, CONSTANT, (NodeValue){.number = token->value.number})
#define _IDENT_N(token)             createNode(NULL, NULL, IDENTIFIER, (NodeValue){.index = getIndex(nt, _IDENT_T(token))})
#define _KWD_N(kwd_type)            createNode(NULL, NULL, KEYWORD, (NodeValue){.keyword_type = kwd_type})
#define _VAR_DECL(token)            createNode(_KWD_N(NUMBER), NULL, VAR_DECLARATION, (NodeValue){.index = getIndex(nt, _IDENT_T(token))});
#define _FUNC_DEF(token)            createNode(_KWD_N(NUMBER), parameters_node, FUNCTION_DEFINITION, (NodeValue){.index = getIndex(nt, _IDENT_T(token))})
#define _FUNC_CALL(token)           createNode(getArguments(tokens, nt), _IDENT_N(token), CALL, DUMMY)
#define _SEMICOLON(statement)       createNode(statement, getProgramm(tokens, nt), KEYWORD, (NodeValue){.keyword_type = SEMICOLON})

#define _PRIORITY0_FUNC(func)       *tokens = _NT;                                                              \
                                    _CHECK_OPEN_BRACKET(syntaxError(EXPRESSION_NOT_FULL));                      \
                                    *tokens = _NT;                                                              \
                                    Node* ret = _##func(getPriority5(tokens, nt));                              \
                                    _CHECK_CLOSE_BRACKET(syntaxError(EXPRESSION_NOT_FULL));                     \
                                    *tokens = _NT;                                                              \
                                    return ret;

#define _PRIORITIES(oper, num1, num2)   if (_TYPE_T(_T) == TokenType::KEYWORD && _KWD_T(_T) == oper){           \
                                        *tokens = _NT;                                                          \
                                        return _##oper(priority##num1##_node, getPriority##num2(tokens, nt));   \
                                        }

#define _SIN(node)                  createNode(NULL, node, KEYWORD, (NodeValue){.keyword_type = SIN})
#define _COS(node)                  createNode(NULL, node, KEYWORD, (NodeValue){.keyword_type = COS})
#define _SQRT(node)                 createNode(NULL, node, KEYWORD, (NodeValue){.keyword_type = SQRT})
#define _NOT(node)                  createNode(NULL, node, KEYWORD, (NodeValue){.keyword_type = NOT})
#define _POW(node_l, node_r)        createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = POW})
#define _MULT(node_l, node_r)       createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = MULT})
#define _DIV(node_l, node_r)        createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = DIV})
#define _ADD(node_l, node_r)        createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = ADD})
#define _SUB(node_l, node_r)        createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = SUB})

#define _EQUAL(node_l, node_r)      createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = EQUAL})
#define _LESS(node_l, node_r)       createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = LESS})
#define _GREATER(node_l, node_r)    createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = GREATER})
#define _L_OR_EQ(node_l, node_r)    createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = L_OR_EQ})
#define _G_OR_EQ(node_l, node_r)    createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = G_OR_EQ})
#define _N_EQ(node_l, node_r)       createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = N_EQ})
#define _AND(node_l, node_r)        createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = AND})
#define _OR(node_l, node_r)         createNode(node_l, node_r, KEYWORD, (NodeValue){.keyword_type = OR})

enum SyntaxError{
    DOUBLE_INITIALIZATION   = 1,
    WRONG_FUNC_DECLARATION  = 2,
    WRONG_VAR_DECLARATION   = 3,
    NO_SEMICOLON            = 4,
    NO_IDENT_IN_NAMETABLE   = 5,
    NO_OPENING_BRACKET      = 6,
    NO_CLOSING_BRACKET      = 7,
    EXPRESSION_NOT_FULL     = 8,
    WRONG_FUNC_CALL         = 9,
};

#endif
