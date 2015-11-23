/*********************************************************************
 * FILE NAME: HeapInterface.h
 * PURPOSE: Interface of a generic Binary Heap ADT
 * AUTHOR: P. Matsakis (CIS2520, Assignment 4)
 * DATE: 13/11/2015
 *********************************************************************/
#include "HeapType.h"

/*********************************************************************
 * FUNCTION NAME: Initialize
 * PURPOSE: Sets a Heap variable to the empty Heap.
 * ARGUMENTS: . The address of the Heap variable to be initialized
 *              (Heap *) 
 *			  . The requested capacity for the Heap (int),
 *              i.e., the maximum number of values that
 *              can be stored in the Heap.
 *            . A pointer to a copy function
 *              ---------------------------- 
 *              PURPOSE: Makes a copy of a value.
 *              ARGUMENTS: . NULL, or the address (void *) of  
 *                           the location in memory where the 
 *                           copy must be stored
 *                         . The address (void *) of the value 
 *                           to be copied
 *              RETURNS: The address (void *) of the location in
 *                       memory where the copy has been stored;
 *					     NULL if no copy could be made
 *              NOTE: If the first argument is NULL, the copy
 *                    function allocates memory for the copy,
 *                    and this memory should later be freed
 *                    by the destroy function below.
 *              -------------------------------
 *            . A pointer to a destroy function
 *              -------------------------------
 *              PURPOSE: Frees memory that may have been 
 *                       allocated by the copy function above.
 *              ARGUMENT: The address of the value to be destroyed
 *                        (void *); this address must have been
 *                        returned by the copy function called
 *                        with NULL as its first argument
 *              -------------------------------
 *            . A pointer to a compare function
 *              -------------------------------
 *              PURPOSE: Compares two values.
 *              ARGUMENTS: . A pointer to a first value (void *)
 *                         . A pointer to a second value (void *)
 *              RETURNS: The integer
 *                       -1 if the 1st value   is less than the 2nd value,
 *                        0 if the two values are equal,
 *                        1 otherwise
 *              -----------------------
 * RETURNS: True (a nonzero integer) if the Heap could be initialized,
 *		    false (0) otherwise
 * NOTE: Initialize is the only function that may be used right
 *       after the declaration of the Heap variable or a call
 *       to Destroy, and it should not be used otherwise.
 *********************************************************************/
extern int Initialize (Heap *H,
					   int capacity,
					   void * (*copyValue) (void *, void *),
					   void (*destroyValue) (void *),
					   int (*compareValues) (void *, void *));

/*********************************************************************
 * FUNCTION NAME: Insert
 * PURPOSE: Inserts a value in a Heap.
 * ARGUMENTS: . The address of the Heap (Heap *)
 *            . The address of the value to be inserted (void *)
 * RETURNS: True (a nonzero integer) if the value could be inserted,
 *		    false (0) otherwise
 *********************************************************************/
extern int Insert (Heap *H, void *I);
/*********************************************************************
 * FUNCTION NAME: Remove
 * PURPOSE: Removes the largest value from a Heap.
 * ARGUMENT: The address of the Heap (Heap *)
 *********************************************************************/
extern void Remove (Heap *H);

/*********************************************************************
 * FUNCTION NAME: Top
 * PURPOSE: Finds the largest value in a Heap.
 * ARGUMENTS: . The address of the Heap (Heap *)
 *            . The address where a copy of the
 *			    largest value should be stored
 *********************************************************************/
extern void Top (Heap *H, void *I);

/*********************************************************************
 * FUNCTION NAME: Full
 * PURPOSE: Determines whether a Heap is full.
 * ARGUMENT: The address of the Heap (Heap *) 
 * RETURNS: True (a nonzero integer) if the Heap is full,
 *          false (0) otherwise
 *********************************************************************/
extern int Full (Heap *H);

/*********************************************************************
 * FUNCTION NAME: Empty
 * PURPOSE: Determines whether a Heap is empty.
 * ARGUMENT: The address of the Heap (Heap *) 
 * RETURNS: True (a nonzero integer) if the Heap is empty,
 *          false (0) otherwise
 *********************************************************************/
extern int Empty (Heap *H);

/*********************************************************************
 * FUNCTION NAME: Destroy
 * PURPOSE: Frees memory that may have been 
 *          allocated by Initialize and Insert.
 * ARGUMENT: The address of the Heap to be destroyed (Heap *) 
 * NOTE: The last function to be called should always be Destroy. 
 *********************************************************************/
extern void Destroy (Heap *H);


