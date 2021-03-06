#pragma once
#include <stdbool.h>
#include <stdlib.h>

//Simple library that handles the info needed for the simulation threads and their respective CPU bursts

typedef enum {
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED,
} State;

typedef struct Burst {
    int cpuTime;
    int ioTime;
} Burst;

typedef struct Thread {
    int threadNo;
    int processNo;
    int ioTime;
    int serviceTime;
    State state;
    int arrivalTime;
    int readyTime;
    int currentBurst;
    int numBursts;
    Burst *bursts[];
} Thread;

Thread *createThread();
