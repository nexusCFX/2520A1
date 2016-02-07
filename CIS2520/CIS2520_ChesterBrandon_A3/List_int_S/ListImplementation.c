/*********************************************************************
 * FILE NAME: ListImplementation.c
 * PURPOSE: Sequential implementation of the Student List ADT.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 1)
 * DATE: 21/09/2015
 * NOTES: . Some functions are static (they are auxiliary functions
 *          with local scope; they are not visible outside the file).
 *        . Only these static functions will need to be modified
 *          if the type of the list items (here, Student) changes. 
 *        . Only the other functions will need to be modified if
 *          the implementation of the List ADT changes.
 *        . As we will see, there is a better way to check whether
 *          the preconditions and postconditions are met.
 *********************************************************************/
int partition(int arr[], int first, int last);
void swap(int *a, int *b);

#include "ListInterface.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define DEBUG

void Initialize (List *L) {
	L->size=0;
#ifdef DEBUG
	if(!Empty(L) || Full(L) || Size(L)!=0) {
		printf("Violated postcondition for Initialize!\n");
		exit(EXIT_FAILURE);
	}
#endif
}


void Insert (Item X, int position, List *L) {
    int i;
#ifdef DEBUG
  	Item Y;
	int oldSize=Size(L);
	if(position<0 || position>oldSize || Full(L)) {
		printf("Violated precondition for Insert!\n");
		exit(EXIT_FAILURE);
	}
#endif
    if (L->size >= position) {
        for (i = (L->size-1); i >= position; i--) {
            L->items[i+1] = L->items[i];
        }
    }
    
    L->items[position] = X;
    L->size++;
#ifdef DEBUG
	Peek(position,L,&Y);
	if(Empty(L) || Size(L)!=oldSize+1) {
		printf("Violated postcondition for Insert!\n");
		exit(EXIT_FAILURE);
	}
#endif
}


void Remove (int position, List *L) {
    int i;
#ifdef DEBUG
	int oldSize=Size(L);
	if(position<0 || position>=oldSize || Empty(L)) {
		printf("Violated precondition for Remove!\n");
		exit(EXIT_FAILURE);
	}
#endif
    
    if (L->size >= position) {
        for (i = position; i < L->size; i++) {
            L->items[i] = L->items[i+1];
        }
        
        L->size--;
    }
#ifdef DEBUG
	if(Full(L) || Size(L)!=oldSize-1) {
		printf("Violated postcondition for Remove!\n");
		exit(EXIT_FAILURE);
	}
#endif
}


int Full (List *L) {
	return L->size==MAXLISTSIZE;
}


int Empty (List *L) {
	return L->size==0;
}


int Size (List *L) {
	return L->size;
}


void Peek (int position, List *L, Item *X) {
#ifdef DEBUG
	if(position<0 || position>=Size(L) || Empty(L)) {
		printf("Violated precondition for Peek!\n");
		exit(EXIT_FAILURE);
	}
#endif
	X = &L->items[position];
}


void Destroy (List *L) {
}

void BubbleSort1(List * L) {
    int temp;
    int i;
    int j;
    for (i = 0; i < L->size - 1; i++) {
        for (j = 0; j < L->size - 1; j++) {
            if (L->items[j] > L->items[j + 1]) {
                temp = L->items[j + 1];
                L->items[j + 1] = L->items[j];
                L->items[j] = temp;
            }
        }
    }
}

/**************************************************************** Quick sort ***********************************/

void QuickSort(int arr[], int p, int r )
{
    if (p < r)
    {
        int q = partition(arr, p, r);
        // Sort the left side or below partition value
        QuickSort(arr, p, q-1);
        // Sort right side or values above the partition value
        QuickSort(arr, q + 1, r);
    }
    
}

int partition(int arr[], int first, int last)
{
    // Store partition or dividing value.
    int  x = arr[last];
    
    // Start at position before insertion point.
    int i = first-1;
    
    // Go through the array from our position first to last-1.
    // Everything below 'x' will be on the left side  of i and everything above x will be on the right side of i
    for (int j = first; j <= last - 1; j++)
    {
        if (arr[j] <= x)
        {
            i++;
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
            //swap(&arr[j], &arr[i]);
        }
    }
    
    // Swap the partition value that divides the left and right array into the middle.
    //swap(&arr[last], &arr[i+1] );
    int temp2 = arr[last];
    arr[last] = arr[i+1];
    arr[i+1] = temp2;
    return i+1;
    
}

void swap(int* a, int* b)
{
    int* tmp = a;
    a = b;
    b = tmp;
}


void Merge(List * A, int first, int middle, int last) {
    int L[MAXLISTSIZE];
    int R[MAXLISTSIZE];
    int i;
    int j;
    int k;
    for (i = 0; i < (middle - first + 1); i++) {
        L[i] = A->items[first + i];
    }
    L[middle - first + 1] = INT_MAX;
    
    for (j = 0; j < (last - middle); j++) {
        R[j] = A->items[middle + j + 1];
    }
    R[last - middle] = INT_MAX;
    i = 0;
    j = 0;
    for (k = first; k < last + 1; k++) {
        if (L[i] < R[j]) {
            A->items[k] = L[i];
            i++;
        } else {
            A->items[k] = R[j];
            j++;
        }
    }
}

void MergeSort(List * L, int first, int last) {
    int middle;
    if (first < last) {
        middle = (first + last) / 2;
        MergeSort(L, first, middle);
        MergeSort(L, middle + 1, last);
        Merge(L, first, middle, last);
    }
}

