#include "ThreadLib.h"
#include "QueueLib.h"

Queue *createQueue() {
    Queue *q = malloc(sizeof(*q));
    q->size = 0;
    return q;
}

void QueueTimeSortInsert(Queue *q, Thread *t) {
    int pos = 0;
    for (int i = 0; i < q->size; i++) {
        if (q->queue[i]->readyTime > t->readyTime) {
            pos = i;
            break;
        } else if (q->queue[i]->readyTime == t->readyTime) {
            if (q->queue[i]->processNo > t->processNo) {
                pos = i;
            } else {
                pos = i + 1;
            }
            break;
        }
        pos = i + 1;
    }
    for (int j = q->size; j >= pos; j--) {
        q->queue[j + 1] = q->queue[j];
    }
    q->queue[pos] = t;
    q->size++;
}

void QueueIDInsert(Queue *q, Thread *t) {
    int pos = 0;
    for (int i = 0; i < q->size; i++) {
        if (q->queue[i]->processNo > t->processNo) {
            pos = i;
            break;
        } else if (q->queue[i]->processNo == t->processNo) {
            if (q->queue[i]->threadNo > t->threadNo) {
                pos = i;
            } else {
                pos = i + 1;
            }
            break;
        }
        pos = i + 1;
    }
    for (int j = q->size; j >= pos; j--) {
        q->queue[j + 1] = q->queue[j];
    }
    q->queue[pos] = t;
    q->size++;
}

void QueueRemoval(Queue *q) {
    for (int i = 0; i < q->size; i++) {
        q->queue[i] = q->queue[i + 1];
    }
    q->size--;
}
