/*********************************************************************
 * FILE NAME: StudentImplementation.c
 * PURPOSE: A second implementation of the Student ADT;
 *          use of assert() for testing the pre/postconditions.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 2)
 * DATE: 28/09/2015
 * NOTES: Debugging is ON by default. To turn it OFF, you must
 *        define the macro NDEBUG before the inclusion of assert.h.
 *        The assert() macro will then have no effect (and will not
 *        even evaluate its argument). NDEBUG can be defined on the
 *        gcc command line instead:
 *           gcc -DNDEBUG -c StudentImplementation.c
 *        There is no need then to modify the source file.
 *        You can also write
 *           gcc $(FLAG) -c StudentImplementation.c
 *        in your makefile and
 *           make -B FLAG=-DNDEBUG
 *        where -B is to unconditionally make all targets.
 *********************************************************************/


#include "StudentInterface.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


void InitializeStudent (char *name, int grade, Student *S) {
	S->name=(char *)malloc(strlen(name)+1);
	strcpy(S->name,name);
	S->grade=grade;
	/* begin postcondition */
	assert(strcmp(name,NameOfStudent(*S))==0 && GradeOfStudent(*S)==grade);
}


char *NameOfStudent (Student S) {
	static char *name=NULL;
	name=realloc(name,strlen(S.name)+1);
	strcpy(name,S.name);
	return name;
}


int GradeOfStudent (Student S) {
	return S.grade;
}


void FreeStudent (Student *S) {
	free(S->name);
}
