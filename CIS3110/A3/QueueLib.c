#include "ProcLib.h"
#include "QueueLib.h"

Queue* createQueue(int size) {
    Queue *q = malloc(sizeof(*q) + sizeof(Proc*)*size);
    q->size = 0;
    return q;
}

void QueueInsert(Queue *q, Proc* p) {
    q->queue[q->size] = p;
    q->size++;
}

void QueueRemove(Queue *q) {
    for (int i = 0; i < q->size; i++) {
        q->queue[i] = q->queue[i + 1];
    }
    q->size--;
}
