
#include <stdlib.h>
#include <stdio.h>
#include "TreeInterface.h"
#include "StudentInterface.h"

void FreeNode(TreeNode* n);
int findHeight(TreeNode* n);
void DestroyNodes(TreeNode* n);
void BalanceTree(Tree* T, TreeNode* n);
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
    if (n == NULL) {
        n = NewNode(NULL);
        n->value = T->copyValue(NULL, I);
        T->size++;
    } else if (T->compareValues(n->value,I) == -1) {
        if(n->right == NULL) {
            n->right = NewNode(T->current);
            n->right->value = T->copyValue(NULL, I);
            T->size++;
        } else {
            InsertNode(T, n->right, I);
        }
    } else if (T->compareValues(n->value,I) == 1) {
        if(n->left == NULL) {
            n->left = NewNode(T->current);
            n->left->value = T->copyValue(NULL, I);
            T->size++;
        } else {
            InsertNode(T, n->left, I);
        }
    }
    T->root = n;
}

/*I'm aware that this code is a mess that could be generalized and thus shortened, 
 but I'm terrified that if I change it at all that it will break and never work again*/
void BalanceTree(Tree* T, TreeNode* n) {
    int balance;
    int left;
    TreeNode* temp;
    TreeNode* parent;
    TreeNode* gParent;
    TreeNode* ggParent;
    left = 0;
    
    if (n->parent == NULL) {
        T->root = n;
        
        return;
    }
    parent = n->parent;
    gParent = parent->parent;
    if (gParent != NULL) {
        ggParent = gParent->parent;
    } else {
        ggParent = NULL;
    }
    if (ggParent && ggParent->left == gParent) {
        left = 1;
    }
    
    if (gParent != NULL) {
        balance = findHeight(gParent->left) - findHeight(gParent->right);
        if (balance < -1) {
            if (n == parent->right) {
                temp = parent->left;
                if (temp != NULL) {
                    temp->parent = gParent;
                }
                parent->left = gParent;
                if (gParent != NULL) {
                    gParent->parent = parent;
                }
                gParent->right = temp;
                if(ggParent){
                    if (left){
                        ggParent->left = parent;
                    } else {
                        ggParent->right = parent;
                    }
                }
                parent->parent = ggParent;
                BalanceTree(T, parent);
            } else if (n == parent->left){
                temp = n->right;
                if (temp != NULL) {
                    temp->parent = parent;
                }
                n->right = parent;
                if (parent != NULL) {
                    parent->parent = n;
                }
                
                parent->left = temp;
                temp = n->left;
                if (temp != NULL) {
                    temp->parent = gParent;
                }
                
                n->left = gParent;
                if (gParent != NULL) {
                    gParent->parent = n;
                }
                
                gParent->right = temp;
                
                if(ggParent){
                    if (left){
                        ggParent->left = n;
                    } else {
                        ggParent->right = n;
                    }
                }
                
                n->parent = ggParent;
                
                BalanceTree(T, n);
            }
        } else if (balance > 1) {
            if (n == parent->left) {
                temp = parent->right;
                if (temp != NULL) {
                    temp->parent = gParent;
                }
                parent->right = gParent;
                if (gParent != NULL) {
                    gParent->parent = parent;
                }
                gParent->left = temp;
                if(ggParent){
                    if (left){
                        ggParent->left = parent;
                    } else {
                        ggParent->right = parent;
                    }
                }
                parent->parent = ggParent;
                BalanceTree(T, parent);
            } else if (n == parent->right){
                temp = n->left;
                if (temp != NULL) {
                    temp->parent = parent;
                }
                n->left = parent;
                if (parent != NULL) {
                    parent->parent = n;
                }
                parent->right = temp;
                temp = n->right;
                if (temp != NULL) {
                    temp->parent = gParent;
                }
                n->right = gParent;
                if (gParent != NULL) {
                    gParent->parent = n;
                }
                gParent->left = temp;
                if(ggParent){
                    if (left){
                        ggParent->left = n;
                    } else {
                        ggParent->right = n;
                    }
                }
                n->parent = ggParent;
                BalanceTree(T, n);
            }
        } else {
            BalanceTree(T, parent);
        }
    }
}

/*If you're not sure that your leftmost leaf is the lowest value your BST is broken*/
int Minimum (Tree *T, void *I) {
    if (T->size == 0) {
        return 0;
    }
    while (T->current->left != NULL) {
        T->current = T->current->left;
    }
    I = T->current->value;
    return 1;
}

int Successor (Tree *T, void *I) {
    
    return 1;
}
