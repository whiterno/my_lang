#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/syntax_tree.h"

static Node* createNodeIn(Node* left, Node* right, NodeType data_type);
static void printNode(Node* node, FILE* fp);

int syntaxTreeCtor(SyntaxTree** tree INIT_ARGS_BET){
    assert(tree);

    (*tree)                 = (SyntaxTree*)calloc(1, sizeof(SyntaxTree));
    (*tree)->root           = NULL;

    INIT_DEBUG_VARS_BET(tree)

    return NO_ERROR;
}

Node* createNode(Node* left, Node* right, NodeType node_type, NodeValue value) {
    Node* node = createNodeIn(left, right, node_type);
    memcpy(&node->value, &value, sizeof(NodeValue));

    return node;
}

Identifier createIdentifier(const char value[]){
    Identifier id = {};

    id.length = strlen(value);

    char* ptr = (char*)calloc(id.length, sizeof(char));
    for (size_t i = 0; i < id.length; i++){
        *(ptr + i) = value[i];
    }

    id.id = ptr;

    return id;
}

static Node* createNodeIn(Node* left, Node* right, NodeType data_type){
    Node* new_node = (Node*)calloc(1, sizeof(Node));

    new_node->left      = left;
    new_node->right     = right;
    new_node->parent    = NULL;
    new_node->type      = data_type;

    return new_node;
}

int countNodes(Node* node, int* amount){
    assert(node);

    if (node->left)     countNodes(node->left, amount);
    if (node->right)    countNodes(node->right, amount);

    (*amount)++;

    return NO_ERROR;
}

int syntaxTreeDtor(SyntaxTree** tree){
    assert(*tree);

    nodesDtor((*tree)->root);
    free(*tree);
    *tree = NULL;

    return NO_ERROR;
}

int nodesDtor(Node* node){
    if (node == NULL) return NO_ERROR;

    if (node->left ==  NULL){
        free(node);

        return NO_ERROR;
    }

    nodesDtor(node->left);
    nodesDtor(node->right);

    free(node);

    return NO_ERROR;
}

Node* makeParents(Node* root, Node* parent){
    if (root == NULL) return root;

    root->parent = parent;

    makeParents(root->left, root);
    makeParents(root->right, root);

    return root;
}

void printTreePrefix(Node* node, FILE* fp){
    if (!node){
        fprintf(fp, "_ ");
        return;
    }

    fprintf(fp, "( ");
    printNode(node, fp);

    printTreePrefix(node->left, fp);
    printTreePrefix(node->right, fp);

    fprintf(fp, ") ");
}

static void printNode(Node* node, FILE* fp){
    switch(node->type){
        case(CONSTANT):             fprintf(fp, "1 %d ", node->value.number); break;
        case(IDENTIFIER):           fprintf(fp, "2 %ld ", node->value.index); break;
        case(KEYWORD):              fprintf(fp, "3 %d ", node->value.keyword_type); break;
        case(FUNCTION_DEFINITION):  fprintf(fp, "4 %ld ", node->value.index); break;
        case(PARAMETERS):           fprintf(fp, "5 "); break;
        case(VAR_DECLARATION):      fprintf(fp, "6 %ld ", node->value.index); break;
        case(CALL):                 fprintf(fp, "7 "); break;
    }
}
