#include "StackInterface.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void Initialize (Stack *S) {
	StackNode *newNode = malloc(sizeof(StackNode));
    newNode->item = 666;
    S->size = 0;
	S->first = newNode;
    S->first->next = NULL;
    
#ifndef DNDEBUG
    assert(Empty(S) == 1 && "List was not empty after initialization");
    assert(Size(S) == 0 && "List wasn't empty right after initialization");
#endif
}

void Push (Item X, Stack *S) {
#ifndef DNDEBUG
    assert(Full(S) == 0 && "List is full");
    int oldSize = S->size;
#endif
    
    StackNode *newNode = malloc(sizeof(StackNode));
    newNode->item = X;
    newNode->next = NULL;
    
    if (Empty(S)) {
        S->first->next = newNode;
        S->size++;
        return;
    }

    StackNode *n = S->first;
	S->size++;
			 
    while (n->next) {
        n = n->next;
    }
    
    n->next = newNode;
    
#ifndef DNDEBUG
    assert(Empty(S) == 0 && "List is still empty; push failure");
    assert(oldSize+1 == S->size && "List size did not increase; push failure");
    Item top;
    Top(S,&top);
    assert(top == X);
#endif
}

void Pop (Stack *S) {
#ifndef DNDEBUG
    assert(Empty(S) == 0 && "List is empty");
    int oldSize = S->size;
#endif
	
    StackNode *n = S->first;
    StackNode *o = n->next;
    
    S->size--;
			 
    while (o->next) {
        n = o;
        o = o->next;
    }

    n->next = NULL;
    free(o);

#ifndef DNDEBUG
    assert(Full(S) == 0 && "Removal failed");
    assert(oldSize-1 == S->size && "List size did not decrease; removal failure");
#endif
}

int Full (Stack *S) {
	return 0;
}

int Empty (Stack *S) {
	return S->size==0;
}

int Size (Stack *S) {
	return S->size;
}

void Top (Stack *S, Item *X) {
#ifndef DNDEBUG
    assert(Empty(S) == 0 && "List is empty, cannot get top");
#endif
    
    StackNode *s = S->first;
    
    while (s->next) {
        s = s->next;
    }
    *X = s->item;
}

void Destroy (Stack *S) {
    if (S) {
        int i = Size(S);
        
        while (i-->1) {
            Pop(S);
        }
        free(S->first);
    }
}

