// Very naive queue implementation using a flexible array member and a size.
// Does not use a moving head counter and it's not a min heap so ops are O(n)
// instead of O(1). It's fast enough for these data sets.
#pragma once
#include "ThreadLib.h"
typedef struct Queue {
    int size;
    Thread* queue[];
} Queue;

//Creates a queue and allocates memory for the base struct. Must also realloc for flexible array later.
Queue *createQueue();

//Inserts into the queue, maintaining its sorted order based on future ready time. Lazy O(n).
void QueueTimeSortInsert(Queue *q, Thread *t);

//Removes the head of a queue, shifts all elements left by one. Lazy O(n).
void QueueRemoval(Queue *q);

//Inserts into the queue, maintaining its sorted order based on process and thread ID. Lazy O(n).
void QueueIDInsert(Queue *q, Thread *t);
