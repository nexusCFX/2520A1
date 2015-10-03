#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "StudentInterface.h"

void InitializeStudent (char *name, int grade, Student *S) {
    strncpy(S->name, name,MAXNAMESIZE-1);
    S->name[MAXNAMESIZE-1] = '\0';
    S->grade = grade;
    
#ifdef DEBUG
    assert(strcmp(S->name,name) == 0 && "Name does not match");
    assert(S->grade == grade && "Grade does not match");
#endif
}

char *NameOfStudent (Student S) {
    static char name[20];
    strcpy(name,S.name);
    return name;
}

int GradeOfStudent (Student S) {
	return S.grade;
}

void FreeStudent (Student *S) {
    /*Should not be used for sequential list as the students are allocated on the stack
    
    if (S) {
        free(S);
        S = 0;
    }*/
}
