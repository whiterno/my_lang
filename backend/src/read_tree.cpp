#include <stdio.h>
#include <assert.h>

#include "utils.h"
#include "syntax_tree.h"

static Node* buildTree(char** tree_txt);
static void readValues(char** tree_txt, Node* node);

Node* readTree(const char* filename){
    assert(filename);

    char* tree_txt  = NULL;
    readFile(filename, &tree_txt);

    printf("%s\n", tree_txt);

    char* old_ptr   = tree_txt;
    Node* root      = buildTree(&tree_txt);
    free(old_ptr);

    return root;
}

static Node* buildTree(char** tree_txt){
    if (**tree_txt == '_'){
        (*tree_txt) += 2;

        return NULL;
    }
    if (**tree_txt == '(' || **tree_txt == ')') (*tree_txt) += 2;
    if (**tree_txt == '_'){
        (*tree_txt) += 2;

        return NULL;
    }

    NodeType type = CONSTANT;
    int shift  = 0;

    sscanf(*tree_txt, "%d %n", &type, &shift);

    printf("tp: %d\n", type);

    (*tree_txt) += shift;

    Node* node = createNode(NULL, NULL, type, (NodeValue){.number = -1});
    readValues(tree_txt, node);

    node->left  = buildTree(tree_txt);
    node->right = buildTree(tree_txt);

    return node;
}

static void readValues(char** tree_txt, Node* node){
    int shift = 0;

    printf("type: %d\n", node->type);

    switch (node->type){
    case (CONSTANT):{
        sscanf(*tree_txt, "%d %n", &node->value.number, &shift);
        (*tree_txt) += shift;
        break;
    }
    case (IDENTIFIER):{
        sscanf(*tree_txt, "%ld %n", &node->value.index, &shift);
        (*tree_txt) += shift;
        break;
    }
    case(KEYWORD):{
        sscanf(*tree_txt, "%d %n", &node->value.keyword_type, &shift);
        printf("shift: %d\n", shift);
        (*tree_txt) += shift;
        break;
    }
    case(FUNCTION_DEFINITION):{
        sscanf(*tree_txt, "%ld %n", &node->value.index, &shift);
        (*tree_txt) += shift;
        break;
    }
    case(VAR_DECLARATION):{
        sscanf(*tree_txt, "%ld %n", &node->value.index, &shift);
        (*tree_txt) += shift;
        break;
    }
    default:
        break;
    }
}
