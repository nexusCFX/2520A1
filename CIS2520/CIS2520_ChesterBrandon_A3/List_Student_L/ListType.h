/*********************************************************************
 * FILE NAME: ListType.h
 * PURPOSE: Concrete data structure definition of Student List.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 2)
 * DATE: 28/09/2015
 * NOTES: The content of this file is the same as for Assignment 1.
 *********************************************************************/


#include "StudentInterface.h"
typedef Student Item;

typedef struct ListNodeTag {
	Item item;
	struct ListNodeTag *next;
} ListNode;

typedef struct {
	int size;
	ListNode *first;
} List;
