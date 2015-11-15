/*********************************************************************
 * FILE NAME: ListImplementation.c
 * PURPOSE: One-way linked implementation of the Student List ADT;
 *          use of assert() for testing the pre/postconditions.
 * AUTHOR: P. Matsakis (CIS2520, Assignment 2)
 * DATE: 28/09/2015
 * NOTES: Debugging is ON by default. To turn it OFF, you must
 *        define the macro NDEBUG before the inclusion of assert.h.
 *        The assert() macro will then have no effect (and will not
 *        even evaluate its argument). NDEBUG can be defined on the
 *        gcc command line instead:
 *           gcc -DNDEBUG -c ListImplementation.c
 *        There is no need then to modify the source file.
 *        You can also write
 *           gcc $(FLAG) -c ListImplementation.c
 *        in your makefile and
 *           make -B FLAG=-DNDEBUG
 *        where -B is to unconditionally make all targets.
 *********************************************************************/


#include "ListInterface.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void copyItem (Item *Y, Item X) {
    InitializeStudent(NameOfStudent(X),GradeOfStudent(X),Y);
}


static void destroyItem (Item *X) {
    FreeStudent(X);
}


static ListNode *moveTo (int position, List *L) {
    int i;
    ListNode *p=L->first;
    for(i=0;i<position;i++) p=p->next;
    return p;
}


void Initialize (List *L) {
    L->size=0;
    L->first=NULL;
}


void Insert (Item X, int position, List *L) {
    ListNode *p, *q;
    
    L->size++;
    q=(ListNode *)malloc(sizeof(ListNode));
    copyItem(&q->item,X);
			 
    if(position==0) {
        q->next=L->first;
        L->first=q;
    }
    else {
        p=moveTo(position-1,L);
        q->next=p->next;
        p->next=q;
    }
}


void Remove (int position, List *L) {
    ListNode *p, *q;
    
    if(position==0) {
        q=L->first;
        L->first=q->next;
    }
    else {
        p=moveTo(position-1,L);
        q=p->next;
        p->next=q->next;
    }
    
    destroyItem(&q->item);
    free(q);
    L->size--;
}


int Full (List *L) {
    return 0;
}


int Empty (List *L) {
    return L->size==0;
}


int Size (List *L) {
    return L->size;
}


void Peek (int position, List *L, Item *X) {
    ListNode *p;
    p=moveTo(position,L);
    copyItem(X,p->item);
}


void Destroy (List *L) {
    int i;
    ListNode *p, *q;
    
    p=L->first;
    for(i=0;i<L->size;i++) {
        q=p;
        p=p->next;
        destroyItem(&q->item);
        free(q);
    }
}

ListNode* ReverseList(ListNode* node, ListNode* prevNode) {
    ListNode* newHead;
    if (node == NULL) {
        return prevNode;
    }
    
    newHead = ReverseList(node->next,node);
    node->next = prevNode;
    return newHead;
}

void Reverse(List* list){
    list->first = ReverseList(list->first,NULL);
}
