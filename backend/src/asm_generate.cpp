#include <stdio.h>

#include "asm_generate.h"

#include "syntax_tree.h"

void generateCode(Node* node){
    if (node == NULL) return;

    if (_TYPE(node) == KEYWORD && _KWD(node) == SEMICOLON){
        generateCode(node->left);
        generateCode(node->right);

        return;
    }

}
