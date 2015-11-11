/*********************************************************************
 * FILE NAME: ListType.h
 * PURPOSE: Concrete data structure definition of Student List.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 1)
 * DATE: 21/09/2015
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
