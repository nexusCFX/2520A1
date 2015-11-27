/*********************************************************************
 * FILE NAME: HeapInterface.h
 * PURPOSE: Interface of a generic Binary Heap ADT
 * AUTHOR: Brandon Chester (CIS2520, Assignment 4)
 * DATE: 26/11/2015
 *********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HeapInterface.h"
#include "StudentInterface.h"

int Initialize (Heap *H, int capacity, void * (*copyValue) (void *, void *), void (*destroyValue) (void *), int (*compareValues) (void *, void *)) {
    H->heap = malloc(capacity*sizeof(void*));
    
    H->capacity = capacity;
    H->numberOfValues = 0;
    H->copyValue = copyValue;
    H->destroyValue = destroyValue;
    H->compareValues = compareValues;
    return 1;
}

int Insert (Heap *H, void *I) {
    void* temp;
    int position;
    H->heap[H->numberOfValues] = H->copyValue(NULL,I);
    H->numberOfValues++;
    position = H->numberOfValues - 1;
    while (position != 0) {
        if (position % 2 != 0) {
            if(H->compareValues(H->heap[position],H->heap[(position-1)/2]) == 1) {
                temp = H->heap[position];
                H->heap[position] = H->heap[(position-1)/2];
                H->heap[(position-1)/2] = temp;
                position = (position-1)/2;
            } else {
                position = 0;
            }
        } else if (position % 2 == 0) {
            if(H->compareValues(H->heap[position],H->heap[(position-2)/2]) == 1) {
                temp = H->heap[position];
                H->heap[position] = H->heap[(position-2)/2];
                H->heap[(position-2)/2] = temp;
                position = (position-2)/2;
            } else {
                position = 0;
            }
        }
    }
    return 1;
}

void Remove (Heap *H) {
    void* temp;
    int position;
    H->destroyValue(H->heap[0]);
    H->numberOfValues--;
    H->heap[0] = H->heap[H->numberOfValues];
    
    position = 0;
    
    while ((2*position+2 ) <= H->numberOfValues - 1) {
        if(H->compareValues(H->heap[2*position + 1],H->heap[2*position + 2]) != 1) {
            if(H->compareValues(H->heap[position],H->heap[2*position + 2]) == -1){
                temp = H->heap[position];
                H->heap[position] = H->heap[2*position + 2];
                H->heap[2*position + 2] = temp;
                position = 2*position + 2;
            } else {
                position = H->numberOfValues;
            }
        } else if(H->compareValues(H->heap[2*position + 1],H->heap[2*position + 2]) == 1){
            if(H->compareValues(H->heap[position],H->heap[2*position + 1]) == -1){
                temp = H->heap[position];
                H->heap[position] = H->heap[2*position + 1];
                H->heap[2*position + 1] = temp;
                position = 2*position + 1;
            } else {
                position = H->numberOfValues;
            }
        }
    }
}

int Full (Heap *H) {
    return H->capacity == H->numberOfValues;
}

int Empty (Heap *H) {
    return H->numberOfValues == 0;
}

void Top (Heap *H, void *I) {
    H->copyValue(I,H->heap[0]);
}

void Destroy (Heap *H) {
    int i;
    for (i = 0; i < H->numberOfValues; i++) {
        H->destroyValue(H->heap[i]);
    }
    H->numberOfValues = 0;
    free(H->heap);
}

