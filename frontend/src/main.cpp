#include <stdio.h>

#include "syntax_tree.h"
#include "syntax_tree_dump.h"
#include "parsing.h"

int main(){
    SyntaxTree* tree = NULL;
    syntaxTreeCtor(&tree INIT_BET(tree));

    makeParents(tree->root, NULL);

    tree->root = parsing("test1.lan");

    printf("Fucking dump\n");

    printTreePrefix(tree->root);

    printf("Here\n");

    syntaxTreeDump(DUMP_BET(tree), 0);

    return 0;
}
