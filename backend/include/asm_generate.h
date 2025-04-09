#ifndef ASM_GENERATE_H
#define ASM_GENERATE_H

#include "syntax_tree.h"

#define _TYPE(node)     node->type

#define _KWD(node)      node->value.keyword_type
#define _INX(node)      node->value.index
#define _NUM(node)      node->value.number

#define _R(node)        node->right
#define _L(node)        node->left
#define _P(node)        node->parent

void generateCode(Node* root, FILE* asm_code);

#endif
