#ifndef DUMP_BET_H
#define DUMP_BET_H

#include "syntax_tree.h"

const int MAX_ATTRIBUTE_SIZE = 32;

//! @brief Dump .dot filename for png creating
const char DUMP_DOT_FILENAME[] = "log/dump.dot";

//! @brief Dump .html filename for html dump with log and images
const char DUMP_HTML_FILENAME[] = "log/dump.html";

//! @brief Max amount of symbols in cmd string for system() call
const int MAX_CMD_SIZE = 64;

struct TreeDumpAttributes{
    char node_color         [MAX_ATTRIBUTE_SIZE];
    char node_shape         [MAX_ATTRIBUTE_SIZE];
    char node_fillcolor     [MAX_ATTRIBUTE_SIZE];

    char edge_arrowhead     [MAX_ATTRIBUTE_SIZE];
    char edge_color         [MAX_ATTRIBUTE_SIZE];

};

const char WHILE_STR[]          = "while";
const char IF_STR[]             = "if";
const char DECLARATION_STR[]    = "int";
const char BREAK_STR[]          = "break";
const char CONTINUE_STR[]       = "continue";
const char RETURN_STR[]         = "return";
const char IN_STR[]             = "input";
const char OUT_STR[]            = "print";

int syntaxTreeDump(SyntaxTree* tree, const char filename[], const char funcname[], const size_t line, int error);

#endif
