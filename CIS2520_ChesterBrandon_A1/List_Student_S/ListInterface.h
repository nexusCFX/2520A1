/*********************************************************************
 * FILE NAME: ListInterface.h
 * PURPOSE: Interface of the Student List ADT
 * AUTHOR: P. Matsakis (CIS2520, Assignment 1)
 * DATE: 16/09/2015
 * NOTES: . If the user includes ListInterface.h in a source file,
 *          they should not include StudentInterface.h. Including 
 *          both header files in the same source file would cause 
 *          a compilation error, because ListInterface.h indirectly 
 *          includes StudentInterface.h, and the type Student would 
 *          therefore be defined twice. (As we will see, however,  
 *          there is a way to go around this.)
 *        . Peek calls InitializeStudent, and it is the user's
 *          responsibility to call FreeStudent when appropriate
 *          (see StudentInterface.h).
 *        . In the following,
 *          the word Item may be replaced by the word Student.
 *********************************************************************/
#include "ListType.h"

/*********************************************************************
 * FUNCTION NAME: Initialize
 * PURPOSE: Sets a List variable to the empty List.
 * ARGUMENTS: The address of the List variable to be initialized 
 *            (List *) 
 * ENSURES (postconditions):
 *          . Empty will return true (a nonzero integer).
 *          . Size will return 0.
 * NOTES: Initialize is the only function that may be used right
 *        after the declaration of the List variable or a call
 *        to Destroy, and it should not be used otherwise.
 *********************************************************************/
extern void Initialize (List *L);

/*********************************************************************
 * FUNCTION NAME: Insert
 * PURPOSE: Inserts an Item in a List.
 * ARGUMENTS: . The Item to be inserted (Item)
 *            . The position in the List
 *              where the Item should be inserted in (int) 
 *            . The address of the List (List *L)
 * REQUIRES (preconditions):
 *            . The position should be a nonnegative integer
 *              not greater than the size of the List.
 *            . The List should not be full.
 * ENSURES: . Empty will return false (0).
 *          . Size will return the first integer greater
 *            than the size of the List before the call.
 *          . Peek in the same position will find
 *            the Item that was inserted.
 *********************************************************************/
extern void Insert (Item X, int position, List *L);

/*********************************************************************
 * FUNCTION NAME: Remove
 * PURPOSE: Removes an Item from a List.
 * ARGUMENTS: . The position in the List
 *              where the Item to be removed is in (int) 
 *            . The address of the List (List *L)
 * REQUIRES: . The position should be a nonnegative integer
 *             lower than the size of the List.
 *           . The List should not be empty.
 * ENSURES: . Full will return false (0).
 *          . Size will return the first integer lower
 *            than the size of the List before the call.
 *********************************************************************/
extern void Remove (int position, List *L);

/*********************************************************************
 * FUNCTION NAME: Full
 * PURPOSE: Determines whether a List is full.
 * ARGUMENTS: The address of the List (List *) 
 * RETURNS: True (a nonzero integer) if the List is full,
 *          false (0) otherwise
 *********************************************************************/
extern int Full (List *L);

/*********************************************************************
 * FUNCTION NAME: Empty
 * PURPOSE: Determines whether a List is empty.
 * ARGUMENTS: The address of the List (List *) 
 * RETURNS: True (a nonzero integer) if the List is empty,
 *          false (0) otherwise
 *********************************************************************/
extern int Empty (List *L);

/*********************************************************************
 * FUNCTION NAME: Size
 * PURPOSE: Finds the size of a List.
 * ARGUMENTS: The address of the List (List *) 
 * RETURNS: The size of the List (int)
 *********************************************************************/
extern int Size (List *L);

/*********************************************************************
 * FUNCTION NAME: Peek
 * PURPOSE: Finds an Item in a List.
 * ARGUMENTS: . The position in the List where the Item is in (int) 
 *            . The address of the List (List *L)
 *            . The address of the Item
 *              the Item found should be copied to
 * REQUIRES: . The position should be a nonnegative integer
 *             lower than the size of the List.
 *           . The List should not be empty.
 *********************************************************************/
extern void Peek (int position, List *L, Item *X);

/*********************************************************************
 * FUNCTION NAME: Destroy
 * PURPOSE: Frees memory that may have been allocated
 *          by Initialize, Insert or Remove.
 * ARGUMENTS: The address of the List to be destroyed (List *) 
 * NOTES: The last function to be called should always be Destroy. 
 *********************************************************************/
extern void Destroy (List *L);


