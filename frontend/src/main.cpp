#include <stdio.h>

#include "syntax_tree.h"
#include "syntax_tree_dump.h"
#include "parsing.h"

int main(){
    SyntaxTree* tree = NULL;
    syntaxTreeCtor(&tree INIT_BET(tree));

    makeParents(tree->root, NULL);

    tree->root = parsing("test1.lan");

    FILE* fp = fopen("tree.txt", "w");
    printTreePrefix(tree->root, fp);
    fclose(fp);

    syntaxTreeDump(DUMP_BET(tree), 0);

    return 0;
}
