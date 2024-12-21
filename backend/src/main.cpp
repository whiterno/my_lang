#include <stdio.h>

#include "read_tree.h"
#include "syntax_tree_dump.h"
#include "asm_generate.h"

int main(){
    SyntaxTree* tree = NULL;
    syntaxTreeCtor(&tree INIT_BET(tree));

    printf("here\n");

    tree->root = readTree("../frontend/tree.txt");

    generateCode(tree->root);
}
