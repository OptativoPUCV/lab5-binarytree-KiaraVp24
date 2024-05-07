#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    //new->lower_than = lower_than;
    TreeMap * arbol = (TreeMap *)malloc(sizeof(TreeMap));
    arbol->root = NULL;
    arbol->current = NULL;
    arbol->lower_than = lower_than;
    return arbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * node = createTreeNode(key, value);
    if (tree->root == NULL) {
        tree->root = node;
        tree->current = node;  
    }
    else {
        TreeNode* current = tree->root;
        while (current != NULL) {
            if (tree->lower_than(key, current->pair->key)) {
                if (current->left == NULL) {
                    current->left = node;
                    node->parent = current;
                    tree->current = node;  
                    break;
                }
                current = current->left;
            }
            else {
                if (!tree->lower_than(current->pair->key, key)) {
                    current->pair->value = value;
                    free(node);
                    tree->current = current;  
                    break;
                }
                else if (current->right == NULL) {
                    current->right = node;
                    node->parent = current;
                    tree->current = node;  
                    break;
                }
                current = current->right;
            }
        }
    }
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL) {
        if (node->parent->left == node) {
            node->parent->left = NULL;
        }
        else {
            node->parent->right = NULL;
        }
    }
    else if (node->left != NULL && node->right == NULL) {
        if (node->parent->left == node) {
            node->parent->left = node->left;
        }
        else {
            node->parent->right = node->left;
        }
        node->left->parent = node->parent;
    }
    else if (node->left == NULL && node->right != NULL) {
        if (node->parent->left == node) {
            node->parent->left = node->right;
        }
        else {
            node->parent->right = node->right;
        }
        node->right->parent = node->parent;
    }
    else {
        TreeNode * min = minimum(node->right);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
        return:
    }
    free(node->pair->key);
    free(node->pair->value);
    free(node->pair);
    free(node);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
