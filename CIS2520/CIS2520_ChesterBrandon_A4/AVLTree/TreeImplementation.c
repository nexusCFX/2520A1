
#include <stdlib.h>
#include <stdio.h>
#include "TreeInterface.h"
#include "StudentInterface.h"

void FreeNode(TreeNode* n);
int findHeight(TreeNode* n);
void DestroyNodes(TreeNode* n);
void BalanceTree(Tree* T, TreeNode* n);
TreeNode* rotateLeft (TreeNode* n, TreeNode* c);
TreeNode* rotateRight (TreeNode* n, TreeNode* c);
void InsertNode (Tree* T, TreeNode* n, void* I);

void Initialize (Tree *T, void * (*copyValue) (void *, void *), void (*destroyValue) (void *), int (*compareValues) (void *, void *)) {
    T->size = 0;
    T->root = NULL;
    T->current = NULL;
    T->copyValue = copyValue;
    T->destroyValue = destroyValue;
    T->compareValues = compareValues;
}

TreeNode* NewNode(TreeNode* P) {
    TreeNode* n = malloc(sizeof(TreeNode));
    n->left = NULL;
    n->right = NULL;
    n->parent = P;
    n->value = NULL;
    return n;
}

void Destroy (Tree *T) {
    TreeNode* temp = T->root;
    DestroyNodes(temp);
}

void DestroyNodes(TreeNode* n) {
    if (n->left == NULL && n->right == NULL) {
        return;
    }
    
    if (n->left != NULL) {
        DestroyNodes(n->left);
    }
    
    if (n->right != NULL) {
        DestroyNodes(n->right);
    }
}

int Balanced (Tree *T) {
    int leftHeight;
    int rightHeight;
    if (T->size > 2){
        leftHeight = findHeight(T->root->left);
        rightHeight = findHeight(T->root->right);
        if ((leftHeight - rightHeight) > 1 || (leftHeight - rightHeight) < -1) {
            return 0;
        }
    }
    return 1;
}

int Height (Tree *T) {
    int height;
    TreeNode* temp = T->root;
    height = findHeight(temp);
    return height;
}

int findHeight(TreeNode* n) {
    int leftHeight;
    int rightHeight;
    leftHeight = 0;
    rightHeight = 0;
    if (n == NULL) {
        return -1;
    }
    if (n->left == NULL && n->right == NULL) {
        return 0;
    }
    
    if (n->left != NULL) {
        leftHeight = findHeight(n->left);
    }
    
    if (n->right != NULL) {
        rightHeight = findHeight(n->right);
    }
    
    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
}

void Insert (Tree *T, void *I) {
    InsertNode(T, T->root, I);
}

void InsertNode (Tree* T, TreeNode* n, void* I) {
    int balance;
    if (n == NULL) {
        n = NewNode(NULL);
        n->value = T->copyValue(NULL, I);
        T->size++;
    } else if (T->compareValues(n->value,I) == -1) {
        if(n->right == NULL) {
            n->right = NewNode(n);
            n->right->value = T->copyValue(NULL, I);
            T->size++;
        } else {
            InsertNode(T, n->right, I);
        }
    } else if (T->compareValues(n->value,I) == 1) {
        if(n->left == NULL) {
            n->left = NewNode(n);
            n->left->value = T->copyValue(NULL, I);
            T->size++;
        } else {
            InsertNode(T, n->left, I);
        }
    }
    T->root = n;
    if (Balanced(T) == 0) {
        balance = findHeight(n->left) - findHeight(n->right);
        if (balance < -1) {
            if(T->compareValues(n->right->value,I) > -1) {
                rotateRight(n->right, n->right->left);
            }
           T->root = rotateLeft(n, n->right);
        } else if (balance > 1) {
            if(T->compareValues(n->left->value,I) < 0) {
                rotateLeft(n->left, n->left->right);
            }
            T->root = rotateRight(n, n->left);
        }
    }
}

TreeNode* rotateLeft (TreeNode* n, TreeNode* c) {
    TreeNode* temp;
    if(n->parent && n == n->parent->left){
        n->parent->left = c;
    } else if (n->parent){
        n->parent->right = c;
    }
    
    c->parent = n->parent;
    n->parent = c;
    temp = c->left;
    if (temp) {
        temp->parent = n;
    }
    n->right = temp;
    c->left = n;
    return c;
}

TreeNode* rotateRight (TreeNode* n, TreeNode* c) {
    TreeNode* temp;
    if(n->parent && n == n->parent->left){
        n->parent->left = c;
    } else if (n->parent){
        n->parent->right = c;
    }
    
    c->parent = n->parent;
    n->parent = c;
    temp = c->right;
    if (temp) {
        temp->parent = n;
    }
    n->left = temp;
    c->right = n;
    return c;
}

int Minimum (Tree *T, void *I) {
    T->current = T->root;
    if (T->size == 0) {
        return 0;
    }
    while (T->current->left != NULL) {
        T->current = T->current->left;
    }
    T->copyValue(I, T->current->value);
    return 1;
}

int Successor (Tree *T, void *I) {
    TreeNode* store;
    TreeNode* oldCurrent;
    if (T->current->right) {
        if (T->current->right->left) {
            T->current = T->current->right->left;
        } else {
            T->current = T->current->right;
        }
        T->copyValue(I, T->current->value);
        return 1;
    }
    store = T->current;
    oldCurrent = T->current;
    T->current = store->parent;
    while (T->current->left != store) {
        if (T->current == T->root) {
            T->current = oldCurrent;
            return 0;
        }
        store = T->current;
        T->current = store->parent;
    }
    T->copyValue(I, T->current->value);
    return 1;
}
