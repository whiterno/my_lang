#ifndef ASM_GENERATE_H
#define ASM_GENERATE_H

#define _TYPE(node)     node->type

#define _KWD(node)      node->value.keyword_type
#define _INX(node)      node->value.index
#define _NUM(node)      node->value.number

void generateCode(Node* root);

#endif
