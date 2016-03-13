#include "ThreadLib.h"
Thread *createThread() {
    Thread *thread = malloc(sizeof(*thread));
    thread->processNo = 0;
    thread->arrivalTime = 0;
    thread->readyTime = 0;
    thread->currentBurst = 0;
    thread->numBursts = 0;
    thread->ioTime = 0;
    thread->serviceTime = 0;
    thread->state = NEW;
    return thread;
}
