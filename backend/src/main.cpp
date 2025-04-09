#include <stdio.h>

#include "read_tree.h"
#include "syntax_tree_dump.h"
#include "asm_generate.h"

int main(){
    SyntaxTree* tree = NULL;
    syntaxTreeCtor(&tree INIT_BET(tree));

    tree->root = readTree("../frontend/tree.txt");

    syntaxTreeDump(DUMP_BET(tree), 0);

    makeParents(tree->root, NULL);

    FILE* asm_code = fopen("code.asm", "w");
    generateCode(tree->root, asm_code);
    fclose(asm_code);
}
