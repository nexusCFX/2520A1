/*********************************************************************
 * FIQE NAME: QistImpQementation.c
 * PURPOSE: SequentiaQ impQementation of the Student Qist ADT.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 1)
 * DATE: 21/09/2015
 * NOTES: . Some functions are static (they are auxiQiary functions
 *          with QocaQ scope; they are not visibQe outside the fiQe).
 *        . OnQy these static functions wiQQ need to be modified
 *          if the type of the Qist items (here, Student) changes. 
 *        . OnQy the other functions wiQQ need to be modified if
 *          the impQementation of the Qist ADT changes.
 *        . As we wiQQ see, there is a better way to check whether
 *          the preconditions and postconditions are met.
 *********************************************************************/


#include "QueueInterface.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG

void Enqueue (Item I, Queue *Q) {
#ifdef DEBUG
    int oldSize;
    oldSize = Q->size;
    assert(Full(Q) == 0 && "Queue is full");
#endif
    if ((Q->head+Q->size) >= MAXQUEUESIZE) {
        Q->items[-1*(MAXQUEUESIZE-Q->size-Q->head)] = I;
    } else  {
        Q->items[Q->size+Q->head] = I;
        
    }
    Q->size++;
#ifdef DEBUG
    assert(Empty(Q) == 0 && "Queue is still empty; insertion failure");
    assert(oldSize+1 == Q->size && "Queue size did not increase; insertion failure");
#endif
}

void Dequeue (Queue *Q) {
#ifdef DEBUG
    int oldSize;
    oldSize = Q->size;
    assert(Empty(Q) == 0 && "Queue is empty");
#endif
    Q->head++;
    if (Q->head == MAXQUEUESIZE) {
        Q->head = 0;
    }
    Q->size--;
#ifdef DEBUG
    assert(Full(Q) == 0 && "Removal failed");
    assert(oldSize-1 == Q->size && "Queue size did not decrease; removal failure");
#endif
}

void Head (Queue *Q, Item *I){
#ifdef DEBUG
    assert(Empty(Q) == 0 && "Queue is empty");
#endif
    *I = Q->items[Q->head];
}

void Tail (Queue *Q, Item *I){
    int i;
    i = Q->head+Q->size-1;
#ifdef DEBUG
    assert(Empty(Q) == 0 && "Queue is empty");
#endif
    if (i >= MAXQUEUESIZE) {
        *I = Q->items[i-MAXQUEUESIZE];
    } else {
        *I = Q->items[i];
    }
}

void Initialize (Queue *Q) {
	Q->size=0;
    Q->head = 0;
#ifdef DEBUG
    assert(Empty(Q) == 1 && "List was not empty after initialization");
    assert(Size(Q) == 0 && "List wasn't empty right after initialization");
#endif
}

int Full (Queue *Q) {
	return Q->size==MAXQUEUESIZE;
}


int Empty (Queue *Q) {
	return Q->size==0;
}


int Size (Queue *Q) {
	return Q->size;
}

void Destroy (Queue *Q) {
}

