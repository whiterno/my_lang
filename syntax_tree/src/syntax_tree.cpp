#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/syntax_tree.h"

static Node* createNodeIn(Node* left, Node* right, NodeType data_type);

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
