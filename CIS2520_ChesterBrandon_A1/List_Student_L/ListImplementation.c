#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "StudentInterface.h"
#include "ListInterface.h"
Node* NewNode(Item X) {
    Node* n = malloc(sizeof(Node));

    n->item = X;
    n->next = NULL; 

    return n;
}

void FreeNode(Node* n) {
    if (n) {
        free(n);
    }
}

void Initialize (List *L) {
    Item s;

	InitializeStudent("Root", 100, &s);

    L->root = NewNode(s);
	L->size = 0;


#ifdef DEBUG
    assert(Empty(L) == 1 && "List was not empty after initialization");
    assert(Size(L) == 0 && "List wasn't empty right after initialization");
#endif
}

void Insert (Item X, int position, List *L) {
#ifdef DEBUG
    assert(position >= 0 && "Position is negative");
    assert(position <= L->size && "Position is greater than list size");
    assert(Full(L) == 0 && "List is full");
    int oldSize = L->size;
#endif
    
    Node* n = L->root;
    Node* newNode = NewNode(X);
    int i;
    
    for (i = 0; i < position && n->next; i++) {
        n = n->next;
    }

    if (i == position) {
        Node* temp = n->next;

        n->next = newNode;
        newNode->next = temp;

        L->size++;
    }
#ifdef DEBUG
    assert(Empty(L) == 0 && "List is still empty; insertion failure");
    assert(oldSize+1 == L->size && "List size did not increase; insertion failure");
    Peek(position, L, &X);
    assert(strcmp(newNode->item.name,X.name) == 0 && "Peek failed, names not identical");
    assert(newNode->item.grade == X.grade && "Peek failed, grades not identical");
#endif
}

void Remove (int position, List *L) {
#ifdef DEBUG
    assert(position >= 0 && "Position is negative");
    assert(position < L->size && "Position is not lower than list size");
    assert(Empty(L) == 0 && "List is empty");
    int oldSize = L->size;
#endif
    
    Node* n = L->root->next;
    Node* prev = L->root;
    int i;
    
    for (i = 0; i < position && n->next; i++) {
        prev = n;
        n = n->next;
    }

    if (i == position) {
        prev->next = n->next;
        L->size--;
        FreeNode(n);
    }
    
#ifdef DEBUG
    assert(Full(L) == 0 && "Removal failed");
    assert(oldSize-1 == L->size && "List size did not decrease; removal failure");
#endif
}

int Full (List *L) {
    return 0;
}

int Empty (List *L) {
    return (L->root->next == NULL);
}

int Size (List *L) {
    int count = 0;
    Node* n = L->root->next;
    
    while (n != NULL) {
        count++;
        n = n->next;
    }
    
    return count;
}

void Peek (int position, List *L, Item *X) {
#ifdef DEBUG
    assert(position >= 0 && "Position is negative");
    assert(position < L->size && "Position is not lower than list size");
    assert(Empty(L) == 0 && "List is empty");
#endif
    
    Node* n = L->root->next;
    int i;
    
    for (i = 0; i < position; i++) {
        n = n->next;
    }
    *X = n->item;
}

void Destroy (List *L) {
    if (L) {
        int i = Size(L);

        while (i-->0) {
            Remove(i, L);
        }

        FreeNode(L->root);
    }
}
