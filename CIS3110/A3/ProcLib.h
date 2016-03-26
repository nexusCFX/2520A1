#pragma once
#include <stdbool.h>
#include <stdlib.h>

//Simple library that handles the info needed for the simulation processes

typedef struct Process {
    int size;
    int memIndex;
    char pid;
    int numLoads;
    int blockEnd;
    bool isLoaded;
} Proc;

Proc *createProcess(int size, char pid);
