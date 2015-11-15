/*********************************************************************
 * FILE NAME: myProgram.c
 * PURPOSE: Test program for Student List implementations.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 1)
 * DATE: 21/09/2015
 * NOTES: This program uses test data recorded in a text file
 *        (see test.txt). 
 *********************************************************************/


#include "ListInterface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

double testSort (List* L, double* min, double* max, double* average, char* sort);
void Merge(List * A, int first, int middle, int last);
void MergeSort(List * L, int first, int last);
void BubbleSort1(List * L);
void BubbleSort2(List * L);

static void showListContent (List *L) {
	int i;
	for(i=0;i<Size(L);i++) {
		printf("%d ",L->items[i]);
	}
    puts("to sort");
}

int main(void) {
	FILE *test;
    List L;
    List L2;
    List L3;
    
    int input;
    double ms;
    double maxMergeTime;
    double minMergeTime;
    double averageMergeTime;
    double maxBubble1Time;
    double minBubble1Time;
    double averageBubble1Time;
    double maxBubble2Time;
    double minBubble2Time;
    double averageBubble2Time;
    
    maxMergeTime = 0;
    averageMergeTime = 0;
    minMergeTime = INT_MAX;
    
    averageBubble1Time = 0;
    minBubble1Time = INT_MAX;
    maxBubble1Time = 0;
    
    averageBubble2Time = 0;
    minBubble2Time = INT_MAX;
    maxBubble2Time = 0;
    
    Initialize(&L);
    Initialize(&L2);
    Initialize(&L3);
    
	test=fopen("test.txt","r");
	while(fscanf(test,"%d",&input)==1) {
        Insert(input,Size(&L),&L);
        Insert(input,Size(&L2),&L2);
        Insert(input,Size(&L3),&L3);
	}
    showListContent(&L);
    ms = testSort(&L,&minBubble1Time,&maxBubble1Time,&averageBubble1Time,"Bubble1");
    printf("BubbleSort1 in %f ms\n",ms);
    ms = testSort(&L2,&minBubble2Time,&maxBubble2Time,&averageBubble2Time,"Bubble2");
    printf("BubbleSort2 in %f ms\n",ms);
    ms = testSort(&L3,&minMergeTime,&maxMergeTime,&averageMergeTime,"Merge");
    printf("MergeSort in %f ms\n",ms);
	fclose(test);
	Destroy(&L);
    Destroy(&L2);
    Destroy(&L3);
	return EXIT_SUCCESS;
}

double testSort (List* L, double* min, double* max, double* average, char* sort) {
    double sec;
    clock_t t1;
    clock_t t2;
    if (strcmp("Bubble1",sort)==0) {
        t1=clock();
        BubbleSort1(L);
        t2=clock();
    } else if (strcmp("Bubble2",sort)==0){
        t1=clock();
        BubbleSort2(L);
        t2=clock();
    } else {
        t1=clock();
        MergeSort(L,0,L->size-1);
        t2=clock();
    }
    sec=(t2-t1)/(double)CLOCKS_PER_SEC;
    sec*=1000;
    return sec;
}

