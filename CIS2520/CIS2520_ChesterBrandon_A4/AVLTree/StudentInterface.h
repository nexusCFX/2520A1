/*********************************************************************
 * FILE NAME: StudentInterface.h
 * PURPOSE: Interface of the Student ADT
 * AUTHOR: P. Matsakis (CIS2520, Assignment 4)
 * DATE: 13/11/2015
 * NOTES: The content of this file is the same as for Assignment 1.
 *********************************************************************/
#include "StudentType.h"

/*********************************************************************
 * FUNCTION NAME: InitializeStudent
 * PURPOSE: Sets the name and grade of a Student.
 * ARGUMENTS: . The name of the Student (char *) 
 *            . The grade of the Student (int) 
 *            . The address of the Student variable
 *              to be initialized (Student *) 
 * ENSURES: . NameOfStudent will return the name of the Student.
 *          . GradeOfStudent will return the grade of the Student.
 * NOTES: . InitializeStudent is the only function that may be used
 *          right after the declaration of the Student variable or a
 *          call to FreeStudent, and it should not be used otherwise.
 *        . Depending on the implementation, two names whose first
 *          n characters are the same (where n is a constant) may
 *          be considered the same.
 *********************************************************************/
extern void InitializeStudent (char *name, int grade, Student *S);

/*********************************************************************
 * FUNCTION NAME: NameOfStudent
 * PURPOSE: Finds the name of a Student.
 * ARGUMENTS: The Student whose name is to be found (Student)
 * RETURNS: The name of the Student (char *) 
 * NOTES: NameOfStudent may return the same memory address
 *        each time it is called.
 *********************************************************************/
extern char *NameOfStudent (Student S);

/*********************************************************************
 * FUNCTION NAME: GradeOfStudent
 * PURPOSE: Finds the grade of a Student.
 * ARGUMENTS: The Student whose grade is to be found (Student) 
 * RETURNS: The grade of the Student (int)
 *********************************************************************/
extern int GradeOfStudent (Student S);

/*********************************************************************
 * FUNCTION NAME: FreeStudent
 * PURPOSE: Frees memory that may have been allocated
 *          by InitializeStudent.
 * ARGUMENTS: The address of the Student to be freed (Student *) 
 * NOTES: The last function to be called should always be FreeStudent. 
 *********************************************************************/
extern void FreeStudent (Student *S);
