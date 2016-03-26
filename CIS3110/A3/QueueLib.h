// Very naive queue implementation using a flexible array member and a size.
// Does not use a moving head counter and it's not a min heap so ops are O(n)
// instead of O(1). It's fast enough for these data sets. Not Pascal approved.
#pragma once
#include "ProcLib.h"
typedef struct Queue {
    int size;
    Proc* queue[];
} Queue;

//Creates a queue and allocates memory for up to 26 elements
Queue *createQueue();

//Inserts into the queue
void QueueInsert(Queue *q, Proc* p);

//Removes the head of a queue, shifts all elements left by one. Lazy O(n).
void QueueRemove(Queue *q);
