
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TreeInterface.h"
#include "StudentInterface.h"

void *copyValue (void *address, void *value);
void destroyValue (void *address);
int compareValues (void *value1, void *value2);
void printTree(TreeNode* T);
void printValue();

int main(void) {
    Tree T;
    FILE *test;
    char s[20];
    Student S;
    int grade;
    
    Initialize (&T, copyValue, destroyValue, compareValues);
    test=fopen("test.txt","r");
    printf("\nInitialize()\n");
    printf("Size=%d, Height=%d, Balanced=",T.size,Height(&T));
    if (Balanced(&T) == 1) {
        printf("YES\n\n");
    } else {
        printf("NO\n\n");
    }
    while(fscanf(test,"%s %d",s,&grade)==2) {
        InitializeStudent(s,grade,&S);
        Insert(&T,&S);
        printf("Insert(%s,%d)\n",s,grade);
        printf("Size=%d, Height=%d, Balanced=",T.size,Height(&T));
        if (Balanced(&T) == 1) {
            printf("YES\n\n");
        } else {
            printf("NO\n\n");
        }
    }
    printTree(T.root);
    puts("");
    fclose(test);
    
    Destroy(&T);
    return 1;
}

void printTree(TreeNode* T) {
    if (T == NULL) {
        return;
    }
    printTree(T->left);
    printValue(T);
    printTree(T->right);
}
    
void printValue (TreeNode* n) {
    Student* S;
    int i;
    int spaces;
    S = (Student*)n->value;
    spaces = 10-strlen(S->name);
    printf("%s",S->name);
    for (i = 0; i < spaces; i++) {
        printf(" ");
    }
    printf("%d%%\n",S->grade);
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

int compareValues (void *value1, void *value2){
    Student* S;
    Student* S2;
    
    S = (Student*)value1;
    S2 = (Student*)value2;
    
    if (S2->grade > S->grade) {
        return -1;
    } else if (S2->grade == S->grade){
        if(strcmp(S->name,S2->name) < 0) {
            return -1;
        } else if(strcmp(S->name,S2->name) == 0){
            return 0;
        }
    }
    return 1;
}
