#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "StudentInterface.h"

void InitializeStudent (char *name, int grade, Student *S) {
    S->name = malloc(sizeof(char)*(strlen(name)+1));
    strcpy(S->name, name);

    S->grade = grade;
    
#ifdef DEBUG
    assert(strcmp(S->name,name) == 0 && "Name does not match");
    assert(S->grade == grade && "Grade does not match");
#endif
}

char *NameOfStudent (Student S) {
    /*Remember to free this memory*/
    char* name = malloc(sizeof(char)*(strlen(S.name)+1));
    strcpy(name, S.name);
    return name;
}

int GradeOfStudent (Student S) {
	return S.grade;
}

void FreeStudent (Student *S) {
    /*Not used for static students, just here for my reference
    if (S) {
        free(S);
        S = 0;
    }
     */
}
