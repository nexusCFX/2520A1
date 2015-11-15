/*********************************************************************
 * FILE NAME: sort.c
 * PURPOSE: Test program for measuring list sort times
 * AUTHOR: Brandon Chester (CIS2520, Assignment 3)
 * DATE: 13/11/2015
 *********************************************************************/


#include "ListInterface.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <assert.h>

void testSort (List* L, double* min, double* max, double* average, char* sort);

int main(int argc, char * argv[]) {
    List L;
    List L2;
    List L3;
    int i;
    int j;
    int numLists;
    int sizeLists;
    int random;
    double maxMergeTime;
    double minMergeTime;
    double averageMergeTime;
    double maxBubble1Time;
    double minBubble1Time;
    double averageBubble1Time;
    double maxBubble2Time;
    double minBubble2Time;
    double averageBubble2Time;
    assert(argc == 3 && "Invalid arguments");
    maxMergeTime = 0;
    averageMergeTime = 0;
    minMergeTime = INT_MAX;
    
    averageBubble1Time = 0;
    minBubble1Time = INT_MAX;
    maxBubble1Time = 0;
    
    averageBubble2Time = 0;
    minBubble2Time = INT_MAX;
    maxBubble2Time = 0;
    
    numLists = atoi(argv[1]);
    sizeLists = atoi(argv[2]);
    
    srand(time(NULL));
    Initialize(&L);
    Initialize(&L2);
    Initialize(&L3);
    
    for (i = 0; i < sizeLists; i++) {
        random = (rand());
        Insert(i, i, &L);
        Insert(i, i, &L2);
        Insert(i, i, &L3);
    }
    testSort(&L,&minBubble1Time,&maxBubble1Time,&averageBubble1Time,"Bubble1");
    testSort(&L2,&minBubble2Time,&maxBubble2Time,&averageBubble2Time,"Bubble2");
    testSort(&L3,&minMergeTime,&maxMergeTime,&averageMergeTime,"Merge");
    
    
    if(numLists > 1){
        for (i = 0; i < sizeLists; i++) {
            random = (rand());
            Insert(sizeLists-i, i, &L);
            Insert(sizeLists-i, i, &L2);
            Insert(sizeLists-i, i, &L3);
        }
        testSort(&L,&minBubble1Time,&maxBubble1Time,&averageBubble1Time,"Bubble1");
        testSort(&L2,&minBubble2Time,&maxBubble2Time,&averageBubble2Time,"Bubble2");
        testSort(&L3,&minMergeTime,&maxMergeTime,&averageMergeTime,"Merge");
    }
    
    
    for (j = 0; j < numLists-2; j++) {
        for (i = 0; i < sizeLists; i++) {
            random = (rand());
            Insert(random, i, &L);
            Insert(random, i, &L2);
            Insert(random, i, &L3);
        }
        testSort(&L,&minBubble1Time,&maxBubble1Time,&averageBubble1Time,"Bubble1");
        testSort(&L2,&minBubble2Time,&maxBubble2Time,&averageBubble2Time,"Bubble2");
        testSort(&L3,&minMergeTime,&maxMergeTime,&averageMergeTime,"Merge");
    }
    
    averageMergeTime = 1000*(averageMergeTime/numLists);
    averageBubble2Time = 1000*(averageBubble2Time/numLists);
    averageBubble1Time = 1000*(averageBubble1Time/numLists);
    
    maxBubble1Time*=1000;
    maxBubble2Time*=1000;
    maxMergeTime*=1000;
    
    minBubble1Time*=1000;
    minBubble2Time*=1000;
    minMergeTime*=1000;
    
    puts("BubbleSort1");
    printf("%f ms\n",minBubble1Time);
    printf("%f ms\n",averageBubble1Time);
    printf("%f ms\n",maxBubble1Time);
    puts("BubbleSort2");
    printf("%f ms\n",minBubble2Time);
    printf("%f ms\n",averageBubble2Time);
    printf("%f ms\n",maxBubble2Time);
    puts("MergeSort");
    printf("%f ms\n",minMergeTime);
    printf("%f ms\n",averageMergeTime);
    printf("%f ms\n",maxMergeTime);
    
    Destroy(&L);
    Destroy(&L2);
    Destroy(&L3);
    return EXIT_SUCCESS;
}

void testSort (List* L, double* min, double* max, double* average, char* sort) {
    double ms;
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
        MergeSort(L,0,L->size);
        t2=clock();
    }
    ms=(t2-t1)/(double)CLOCKS_PER_SEC;
    
    if (ms < *min) {
        *min = ms;
    }
    if (ms > *max) {
        *max = ms;
    }
    *average += ms;
    L->size = 0;
}
