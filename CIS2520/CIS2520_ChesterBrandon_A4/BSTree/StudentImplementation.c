/*********************************************************************
 * FILE NAME: StudentImplementation.c
 * PURPOSE: An implementation of the Student ADT.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 4)
 * DATE: 13/11/2015
 * NOTES: The content of this file is the same as for Assignment 1.
 *********************************************************************/


#include "StudentInterface.h"
#include <string.h>
#include <stdlib.h>


void InitializeStudent (char *name, int grade, Student *S) {
	S->name=(char *)malloc(strlen(name)+1);
	strcpy(S->name,name);
	S->grade=grade;
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
