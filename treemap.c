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

TreeMap * createTreeMap(int (*lt) (void* key1, void* key2)) {
    //new->lower_than = lower_than;
    TreeMap * map = (TreeMap *)malloc(sizeof(TreeMap));
    map->root = NULL;
    map->current = NULL;
    map->lower_than = lt;
    return map;
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
    if (node == NULL) {
        return;
    }
    TreeNode* parent = node->parent;
    TreeNode* leftChild = node->left;
    TreeNode* rightChild = node->right;
    if (leftChild == NULL && rightChild == NULL) {
        if (parent == NULL) {
            tree->root = NULL;
        } else if (parent->left == node) {
            parent->left = NULL;
        } else {
            parent->right = NULL;
        }
    }
    else if (leftChild != NULL && rightChild == NULL) {
        if (parent == NULL) {
            tree->root = leftChild;
        } else if (parent->left == node) {
            parent->left = leftChild;
        } else {
            parent->right = leftChild;
        }
        leftChild->parent = parent;
    }
    else if (leftChild == NULL && rightChild != NULL) {
        if (parent == NULL) {
            tree->root = rightChild;
        } else if (parent->left == node) {
            parent->left = rightChild;
        } else {
            parent->right = rightChild;
        }
        rightChild->parent = parent;
    }
    else {
        TreeNode* min = minimum(rightChild);
        node->pair->key = min->pair->key;
        node->pair->value = min->pair->value;
        removeNode(tree, min);
        return;
    }
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
    TreeNode* current = tree->root;
    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* current = tree->root;
    TreeNode* ub = NULL;
    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            ub = current;
            break;
        }
        else if (tree->lower_than(key, current->pair->key)) {
            ub = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (ub == NULL) {
        return NULL;
    } else {
        return ub->pair;
    }
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* node = minimum(tree->root);
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* current = tree->current;
    if (current == NULL) return NULL;
    if (current->right != NULL) {
        tree->current = minimum(current->right);
        return tree->current->pair;
    }
    TreeNode* parent = current->parent;
    while (parent != NULL && current == parent->right) {
        current = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    return parent == NULL ? NULL : parent->pair;
}
