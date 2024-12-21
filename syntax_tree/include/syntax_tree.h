#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <stdio.h>

#define INIT_BET(syntaxTree) , #syntaxTree, __FILE__, __func__, __LINE__
#define INIT_ARGS_BET , const char* name, const char* filename, const char* funcname, size_t line
#define INIT_DEBUG_VARS_BET(syntaxTree) (*syntaxTree)->name = name; (*syntaxTree)->filename = filename; (*syntaxTree)->funcname = funcname; (*syntaxTree)->line = line;
#define DUMP_BET(syntaxTree) syntaxTree, __FILE__, __func__, __LINE__

enum NodeType{
    CONSTANT            = 1,
    IDENTIFIER          = 2,
    KEYWORD             = 3,
    FUNCTION_DEFINITION = 4,
    PARAMETERS          = 5,
    VAR_DECLARATION     = 6,
    CALL                = 7,
};

enum Keyword{
    IF          = 11,
    WHILE       = 12,
    ASSIGN      = 13,
    DECLARATION = 14,

    SIN         = 21,
    COS         = 22,
    FLOOR       = 23,
    ADD         = 24,
    SUB         = 25,
    MULT        = 26,
    DIV         = 27,
    SQRT        = 28,
    POW         = 29,

    EQUAL       = 31,
    LESS        = 32,
    GREATER     = 33,
    L_OR_EQ     = 34,
    G_OR_EQ     = 35,
    N_EQ        = 36,
    AND         = 37,
    OR          = 38,
    NOT         = 39,

    SEMICOLON   = 41,
    COMMA       = 42,
    BRACKET_CL  = 43,
    BRACKET_OP  = 44,
    BRACE_CL    = 45,
    BRACE_OP    = 46,

    NUMBER      = 51,

    IN          = 61,
    OUT         = 62,

    RETURN      = 71,
    BREAK       = 72,
    CONTINUE    = 73,
    ABORT       = 74,
};

struct Identifier{
    const char* id;
    size_t length;
};

union NodeValue{
    Identifier      name;
    size_t          index;

    int             number;
    Keyword         keyword_type;
};

struct Node{
    NodeType    type;
    NodeValue   value;

    Node*       left;
    Node*       right;
    Node*       parent;
};

struct SyntaxTree{
    const char* name;           ///< tree's name
    const char* filename;       ///< filename where tree was initialized
    const char* funcname;       ///< funcname where tree was initialized
    size_t line;                ///< serial number of line where tree was initialized

    Node* root;                 ///< pointer on tree's root
};

enum ErrorsSyntaxTree{
    NO_ERROR,
    NO_SUCH_NODE_TYPE,
    NULL_VALUE_INSERTED,
    FILE_NOT_OPEN,
    CYCLE_IN_TREE,
};

int syntaxTreeCtor(SyntaxTree** tree INIT_ARGS_BET);
Node* createNode(Node* left, Node* right, NodeType data_type, NodeValue value);
Identifier createIdentifier(const char value[]);
int countNodes(Node* node, int* amount);
int syntaxTreeDtor(SyntaxTree** tree);
int nodesDtor(Node* node);
Node* makeParents(Node* root, Node* parent);
void printTreePrefix(Node* node);

#endif
