#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "QueueLib.h"

// Handles the actual simulation
void CPUSimulator(Queue *readyQueue, int sameProcSwitch, int difProcSwitch,
                  bool isDetailed, bool isRR, bool isVerbose, int quantum);

// Parses the input file into thread, burst, and queue structs
Queue *parseFile(int numProcesses);

// Outputs statistics after CPUSim finishes.
void outputStats(Queue *finishQueue, int clock, int cpuTime, int idleTime,
                 bool isDetailed);

// Moves threads from wait queue to ready queue and outputs verbose msgs
void moveToReadyQueue(Queue *readyQueue, Queue *waitQueue, int clock,
                      bool isVerbose);

// Checks the ready queue to see if any processes marked as new are ready
void markProcessAsReady(Queue *readyQueue, int clock, bool isVerbose);

int main(int argc, char *argv[]) {
    bool isVerbose = false;
    bool isDetailed = false;
    bool isRR = false;
    int quantum = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            isVerbose = true;
        }
        if (strcmp(argv[i], "-d") == 0) {
            isDetailed = true;
        }
        if (strcmp(argv[i], "-r") == 0) {
            isRR = true;
            quantum = atoi(argv[i + 1]);
        }
    }

    int numProcesses = 0;
    int sameProcSwitch = 0;
    int difProcSwitch = 0;

    scanf("%d %d %d", &numProcesses, &sameProcSwitch, &difProcSwitch);
    getchar();

    Queue *readyQueue = parseFile(numProcesses);
    CPUSimulator(readyQueue, sameProcSwitch, difProcSwitch, isDetailed, isRR,
                 isVerbose, quantum);

    return 1;
}

Queue *parseFile(int numProcesses) {
    Queue *readyQueue = createQueue();
    for (int i = 0; i < numProcesses; i++) {
        int processNum;
        int numThreads;
        scanf("%d %d", &processNum, &numThreads);
        getchar();
        readyQueue =
            realloc(readyQueue,
                    sizeof(*readyQueue) +
                        sizeof(Thread *) * (readyQueue->size + numThreads + 1));
        for (int j = 0; j < numThreads; j++) {
            Thread *newThread = createThread();
            newThread->threadNo = j;
            scanf("%*d %d %d", &newThread->arrivalTime, &newThread->numBursts);
            newThread->readyTime = newThread->arrivalTime;
            newThread->processNo = processNum;
            getchar();

            newThread =
                realloc(newThread, sizeof(*newThread) +
                                       sizeof(Burst *) * newThread->numBursts);

            for (int k = 0; k < newThread->numBursts; k++) {
                newThread->bursts[k] = malloc(sizeof(Burst));

                char burstInput[100];
                char *token;
                fgets(burstInput, 99, stdin);
                burstInput[strlen(burstInput) - 1] = '\0';
                token = strtok(burstInput, " ");
                token = strtok(NULL, " ");
                newThread->bursts[k]->cpuTime = atoi(token);

                token = strtok(NULL, " ");
                if (token == NULL) {
                    newThread->bursts[k]->ioTime = 0;
                } else {
                    newThread->bursts[k]->ioTime = atoi(token);
                }
                newThread->ioTime += newThread->bursts[k]->ioTime;
                newThread->serviceTime += newThread->bursts[k]->cpuTime;
            }

            QueueTimeSortInsert(readyQueue, newThread);
        }
    }
    return readyQueue;
}

void CPUSimulator(Queue *readyQueue, int sameProcSwitch, int difProcSwitch,
                  bool isDetailed, bool isRR, bool isVerbose, int quantum) {

    // Wait queue is sorted by ready times, finish queue by process and then by
    // thread number
    Queue *waitQueue = createQueue();
    Queue *finishQueue = createQueue();

    // Required to allocate for flexible array member
    finishQueue = realloc(finishQueue, sizeof(*finishQueue) +
                                           sizeof(Thread *) * readyQueue->size);
    waitQueue = realloc(waitQueue, sizeof(*waitQueue) +
                                       sizeof(Thread *) * readyQueue->size);
    int clock = 0;
    int idleTime = 0;
    int cpuTime = 0;
    bool unfinishedBurst = false;

    // Loop until all threads are in finish queue
    while (readyQueue->size > 0 || waitQueue->size > 0) {

        // Check if any of the threads in the "ready" queue are actually new
        // state and put a message
        markProcessAsReady(readyQueue, clock, isVerbose);

        // Make a new thread pointer that is easier to work with
        Thread *currThread = readyQueue->queue[0];

        if (isVerbose) {
            printf("At time %d: Thread %d of Process %d moves from ready to "
                   "running\n",
                   clock, currThread->threadNo + 1, currThread->processNo);
        }
        
        int currBurst = currThread->currentBurst;

        // If we're doing round robin and the quantum is smaller than the burst
        if (isRR && currThread->bursts[currBurst]->cpuTime > quantum) {
            clock += 50;
            cpuTime += 50;
            currThread->bursts[currBurst]->cpuTime -= quantum;
            unfinishedBurst = true;
        } else {
            // Not round robin, or quantum is big enough to do the whole burst
            clock += currThread->bursts[currBurst]->cpuTime;
            cpuTime += currThread->bursts[currBurst]->cpuTime;
            currThread->currentBurst++;
            currBurst++;
        }

        // Check if any blocked / "ready" need to be moved to main queue
        moveToReadyQueue(readyQueue, waitQueue, clock, isVerbose);

        // Go over the ready queue and mark any "new" processes as ready if the
        // clock has passed their arrival time
        markProcessAsReady(readyQueue, clock, isVerbose);

        // If we've finished all our bursts
        if (currBurst == currThread->numBursts) {
            currThread->readyTime = clock;
            if (isVerbose) {
                printf(
                    "At time %d: Thread %d of Process %d moves from running to "
                    "terminated\n",
                    clock, currThread->threadNo + 1, currThread->processNo);
                currThread->state = TERMINATED;
            }
            QueueIDInsert(finishQueue, currThread);
            QueueRemoval(readyQueue);
        } else {
            // If the burst was left unfinished in RR don't move to next burst
            if (!unfinishedBurst) {
                currBurst--;
            }
            if (isVerbose) {
                // If ioTime isn't zero we move to the blocked state
                if (currThread->bursts[currBurst]->ioTime != 0) {
                    printf("At time %d: Thread %d of Process %d moves from "
                           "running to blocked\n",
                           clock, currThread->threadNo + 1,
                           currThread->processNo);
                    currThread->state = BLOCKED;
                } else { // If ioTime is zero we move back to ready
                    printf("At time %d: Thread %d of Process %d moves from "
                           "running to ready\n",
                           clock, currThread->threadNo + 1,
                           currThread->processNo);
                    currThread->state = READY;
                }
            }
            currThread->readyTime =
                clock + currThread->bursts[currBurst]->ioTime;

            unfinishedBurst = false;
            QueueTimeSortInsert(waitQueue, currThread);
            QueueRemoval(readyQueue);
        }

        // If both queues are done we break the loop
        if (readyQueue->size == 0) {
            if (waitQueue->size == 0) {
                continue;
            }
            // If the wait queue isn't empty but the ready queue is just fast
            // forward the clock to when the next process can be ready
            Thread *t = waitQueue->queue[0];
            clock = t->readyTime;
            QueueTimeSortInsert(readyQueue, t);
            QueueRemoval(waitQueue);
        }
        // If these two attributes match then we have the same thread sitting in
        // the CPU continually, so there's no context switch overhead
        if (readyQueue->queue[0]->processNo == currThread->processNo) {
            if (readyQueue->queue[0]->threadNo == currThread->threadNo) {
                continue;
            } else {
                clock += sameProcSwitch;
                idleTime += sameProcSwitch;
            }
        } else {
            clock += difProcSwitch;
            idleTime += difProcSwitch;
        }
    }
    outputStats(finishQueue, clock, cpuTime, idleTime, isDetailed);
}

void markProcessAsReady(Queue *readyQueue, int clock, bool isVerbose) {
    for (int i = 0; i < readyQueue->size; i++) {
        if (isVerbose && readyQueue->queue[i]->state == NEW &&
            clock >= readyQueue->queue[i]->readyTime) {
            printf("At time %d: Thread %d of Process %d moves from new to "
                   "ready\n",
                   readyQueue->queue[i]->readyTime,
                   readyQueue->queue[i]->threadNo + 1,
                   readyQueue->queue[i]->processNo);

            readyQueue->queue[i]->state = READY;
        }
    }
}

void moveToReadyQueue(Queue *readyQueue, Queue *waitQueue, int clock,
                      bool isVerbose) {

    // Iterate the wait queue, move all processes with a ready time less than or
    // equal to the clock back to the ready queue
    for (int i = 0; i < waitQueue->size; i++) {
        Thread *waitingThread = waitQueue->queue[i];
        if (waitingThread->readyTime <= clock) {
            if (isVerbose && waitingThread->state == BLOCKED) {
                printf("At time %d: Thread %d of Process %d moves from "
                       "blocked to ready\n",
                       waitingThread->readyTime, waitingThread->threadNo + 1,
                       waitingThread->processNo);
                waitingThread->state = READY;
            }
            QueueTimeSortInsert(readyQueue, waitingThread);
            QueueRemoval(waitQueue);
        } else {
            break;
        }
    }
}

void outputStats(Queue *finishQueue, int clock, int cpuTime, int idleTime,
                 bool isDetailed) {

    float turnAroundTime = 0;
    float utilization = (float)cpuTime;
    utilization = utilization / (float)clock;
    utilization = utilization * 100;

    for (int i = 0; i < finishQueue->size; i++) {
        Thread *t = finishQueue->queue[i];
        turnAroundTime += (t->readyTime - t->arrivalTime);
    }

    turnAroundTime = (turnAroundTime / finishQueue->size);
    printf("Total Time Required = %d units\n", clock);
    printf("Average Turnaround Time is %0.1f units\n", turnAroundTime);
    printf("CPU Utilization is is %0.1f%%\n", utilization);

    if (isDetailed) {
        for (int i = 0; i < finishQueue->size; i++) {
            Thread *t = finishQueue->queue[i];
            printf("Thread %d of Process %d:\n", t->threadNo + 1, t->processNo);
            printf("  arrival time: %d\n", t->arrivalTime);
            printf("  service time: %d units,", t->serviceTime);
            printf("I/O time: %d units,", t->ioTime);
            printf("turnaround time: %d units,",
                   (t->readyTime - t->arrivalTime));
            printf("finish time: %d units\n", t->readyTime);
        }
    }
}
