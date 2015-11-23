
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HeapInterface.h"
#include "StudentInterface.h"

void *copyValue (void *address, void *value);
void destroyValue (void *address);
int compareValues (void *value1, void *value2);
void printHeap(Heap* H);

int main(int argc, char * argv[]) {
    Heap H;
    FILE *test;
    char s[20];
    Student S;
    Student* SPtr;
    int grade;
    int arg;
    int i;
    void** topVals;
    
    assert(argc == 2 && "Invalid arguments");
    arg = atoi(argv[1]);
    topVals = malloc(arg*sizeof(void*));
    Initialize (&H, 100, copyValue, destroyValue, compareValues);
    test=fopen("test.txt","r");
    while(fscanf(test,"%s %d",s,&grade)==2) {
        InitializeStudent(s,grade,&S);
        Insert(&H,&S);
    }
    fclose(test);
    
    assert(arg <= H.numberOfValues && "The heap does not contain that many students.");
    
    for (i = 0; i < arg; i++) {
        topVals[i] = malloc(30);
        Top(&H, topVals[i]);
        Remove(&H);
        SPtr = (Student*)topVals[i];
        printf("Name %s grade %d\n",SPtr->name,SPtr->grade);
    }
    Destroy(&H);
    return 1;
}

void * copyValue (void *address, void *value) {
    if (address == NULL) {
        address = malloc(30);
    }
     
    memcpy(address, value, 30);
    return address;
}

void destroyValue (void *address) {
    if(address != NULL){
        free(address);
    }
}

/*void printHeap(Heap* H) {
    int i;
    Student* S2;
    puts("");
    for (i = 0; i < H->numberOfValues; i++) {
        S2 = (Student*)H->heap[i];
        printf("Name %s grade %d\n",S2->name,S2->grade);
    }
}*/

int compareValues (void *value1, void *value2){
    Student* S;
    Student* S2;
    
    S = (Student*)value1;
    S2 = (Student*)value2;
    
    if (S2->grade > S->grade) {
        return -1;
    } else if (S2->grade == S->grade){
        return 0;
    }
    return 1;
}
