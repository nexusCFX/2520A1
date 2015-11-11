#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ListInterface.h"

void Initialize (List *L) {
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
    int i;
    
    if (L->size >= position) {
        for (i = (L->size-1); i >= position; i--) {
            L->items[i+1] = L->items[i];
        }
    }

    L->items[position] = X;
    L->size++;
    
#ifdef DEBUG
    assert(Empty(L) == 0 && "List is still empty; insertion failure");
    assert(oldSize+1 == L->size && "List size did not increase; insertion failure");
    Peek(position, L, &X);
    assert(strcmp(L->items[position].name,X.name) == 0 && "Peek failed, names not identical");
    assert(L->items[position].grade == X.grade && "Peek failed, grades not identical");
#endif
}

void Remove (int position, List *L) {
#ifdef DEBUG
    assert(position >= 0 && "Position is negative");
    assert(position < L->size && "Position is not lower than list size");
    assert(Empty(L) == 0 && "List is empty");
    int oldSize = L->size;
#endif
    int i;
    
    if (L->size >= position) {
        for (i = position; i < L->size; i++) {
            L->items[i] = L->items[i+1];
        }

        L->size--;
    }
    
#ifdef DEBUG
    assert(Full(L) == 0 && "Removal failed");
    assert(oldSize-1 == L->size && "List size did not decrease; insertion failure");
#endif
}

int Full (List *L) {
    return (L->size == MAXLISTSIZE);
}

int Empty (List *L) {
    return (L->size == 0);
}

int Size (List *L) {
	return L->size;
}

void Peek (int position, List *L, Item *X) {
#ifdef DEBUG
    assert(position >= 0 && "Position is negative");
    assert(position < L->size && "Position is not lower than list size");
    assert(Empty(L) == 0 && "List is empty");
#endif
    
    *X = L->items[position];
}

void Destroy (List *L) {
    /*Do not use this with static allocated list
    if (L) {
        free(L);
        L = 0;
    }*/
}
