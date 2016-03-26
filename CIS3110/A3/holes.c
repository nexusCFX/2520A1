#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "QueueLib.h"

// Handles the actual simulation for first fit
void FirstFitSim(Queue *memQueue);

// Handles the actual simulation for worst fit
void WorstFitSim(Queue *memQueue);

// Handles the actual simulation for best fit
void BestFitSim(Queue *memQueue);

// Handles the actual simulation for next fit
void NextFitSim(Queue *memQueue);

// Moves threads from wait queue to ready queue and outputs verbose msgs
void swap(Queue *memQueue, Queue *swapQueue, char mem[]);


int main(int argc, char *argv[]) {
    FILE *f = fopen(argv[1], "r");
    char pid;
    int size;
    Queue *waitQueue = createQueue(26);

    while (fscanf(f, " %c %d", &pid, &size) == 2) {
        if (waitQueue->size > 0 && waitQueue->size % 26 == 0) {
            waitQueue =
                realloc(waitQueue, sizeof(*waitQueue) +
                                       sizeof(Proc *) * (waitQueue->size + 20));
        }
        QueueInsert(waitQueue, createProcess(size, pid));
    }
    
    //Make four additional queues which will point to the elements in the order they were read
    Queue *worstQueue = createQueue(waitQueue->size);
    Queue *bestQueue = createQueue(waitQueue->size);
    Queue *nextQueue = createQueue(waitQueue->size);
    Queue *freeQueue = createQueue(waitQueue->size);
    
    for (int i = 0; i < waitQueue->size; i++) {
        QueueInsert(worstQueue, createProcess(waitQueue->queue[i]->size,
                                              waitQueue->queue[i]->pid));
                                              
        QueueInsert(bestQueue, createProcess(waitQueue->queue[i]->size,
                                             waitQueue->queue[i]->pid));
                                             
        QueueInsert(nextQueue, createProcess(waitQueue->queue[i]->size,
                                             waitQueue->queue[i]->pid));
                                             
        QueueInsert(freeQueue, createProcess(waitQueue->queue[i]->size,
                                             waitQueue->queue[i]->pid));
    }
    
    
    worstQueue->size = waitQueue->size;
    bestQueue->size = waitQueue->size;
    nextQueue->size = waitQueue->size;
    freeQueue->size = waitQueue->size;

    fclose(f);
    
    //Run all simulations
    FirstFitSim(waitQueue);
    WorstFitSim(worstQueue);
    BestFitSim(bestQueue);
    NextFitSim(nextQueue);

    //Since we're just handling pointers we only need to free the freeQueue
    for (int i = 0; i < freeQueue->size; i++) {
        free(freeQueue->queue[i]);
    }
    return 1;
}

void FirstFitSim(Queue *memQueue) {
    puts("Now simulating using First Fit memory allocation");

    //The memory is simulated using a 128 element array of characters.
    //The character - signifies free memory. When memory is taken, the spaces
    //will be filled with the PID char of the respective process in memory
    char mem[128];
    int numInMem = 0;
    int numLoads = 0;
    int memUsageAvg = 0;
    int numInMemAvg = 0;
    int numHolesAvg = 0;
    int printAvg = 0;

    //Create a queue to hold items that are unloaded 3 times
    Queue *finishQueue = createQueue(memQueue->size);
    
    //Initialize our memory to the - value
    for (int i = 0; i < 128; i++) {
        mem[i] = '-';
    }

    while (numInMem != memQueue->size) {
        bool isFree = false;
        Proc *proc = NULL;
        int blockSize = 0;
        int startOfBlock = 0;

        // Find the first process that isn't loaded into memory
        for (int i = 0; i < memQueue->size; i++) {
            if (memQueue->queue[i]->isLoaded == false) {
                proc = memQueue->queue[i];
                break;
            }
        }

        //Traverse memory array to find first hole that is large enough
        for (int i = 0; i < 128; i++) {
            
            //If this is true, we've found the end of a block
            if (isFree && mem[i] != '-') {
                isFree = false;
                if (i - startOfBlock > blockSize) {
                    blockSize = i - startOfBlock;
                }
            }
            
            //If this is true, we've found the beginning of a new block
            if (!isFree && mem[i] == '-') {
                isFree = true;
                startOfBlock = i;
            }
            
            //This handles the case where we hit the end of the array
            if (i == 127 && isFree) {
                blockSize = 127 - startOfBlock;
            }

            //If this is true, we can load in the process
            if (blockSize >= proc->size) {
                for (int i = startOfBlock; i < (proc->size + startOfBlock);
                     i++) {
                    mem[i] = proc->pid;
                }
                proc->isLoaded = true;
                proc->numLoads++;
                proc->memIndex = startOfBlock;
                numInMem++;
                numLoads++;
                int numHoles = 0;
                bool inHole = false;
                int memUsage = 0;
                
                //Gather some statistics
                for (int j = 0; j < 128; j++) {
                    if (mem[j] != '-') {
                        memUsage++;
                    }
                    if (!inHole && mem[j] == '-') {
                        numHoles++;
                        inHole = true;
                    }
                    if (inHole && mem[j] != '-') {
                        inHole = false;
                    }
                }
                
                numInMemAvg += numInMem;
                memUsageAvg += memUsage;
                numHolesAvg += numHoles;
                
                memUsage = ((100 * memUsage) / 128);
                printAvg = (((100 * memUsageAvg) / 128) / numLoads);
                
                //Output stats
                printf("%c loaded, #process = %d, #holes = %d, %%memusage = "
                       "%d, cumulative %%mem = %d\n",
                       proc->pid, numInMem, numHoles, memUsage, printAvg);
                break;
            }
        }

        //If we couldn't load, we need to evict programs until we can
        if (proc->isLoaded == false) {
            if (memQueue->queue[0]->numLoads == 3) {
                swap(memQueue, finishQueue, mem);
            } else {
                swap(memQueue, memQueue, mem);
            }
            numInMem--;
        }
    }

    //Print final statistics
    printAvg = (((100 * memUsageAvg) / 128) / numLoads);
    printf("Total loads = %d, average #processes = %0.1f, average #holes = "
           "%0.1f, cumulative %%mem = %d\n",
           numLoads, (float)((float)numInMemAvg / (float)numLoads),
           (float)((float)numHolesAvg / (float)numLoads), printAvg);
}

void WorstFitSim(Queue *memQueue) {
    printf("\nNow simulating using Worst Fit memory allocation\n");
    char mem[128];
    int numInMem = 0;
    int numLoads = 0;
    int memUsageAvg = 0;
    int numInMemAvg = 0;
    int numHolesAvg = 0;
    int chosenStart = 0;
    int printAvg = 0;

    Queue *finishQueue = createQueue(memQueue->size);
    for (int i = 0; i < 128; i++) {
        mem[i] = '-';
    }

    while (numInMem != memQueue->size) {
        bool isFree = false;
        Proc *proc = NULL;
        int blockSize = 0;
        int startOfBlock = 0;

        for (int i = 0; i < memQueue->size; i++) {
            if (memQueue->queue[i]->isLoaded == false) {
                proc = memQueue->queue[i];
                break;
            }
        }

        for (int i = 0; i < 128; i++) {
            if (isFree && mem[i] != '-') {
                isFree = false;
                if (i - startOfBlock > blockSize) {
                    blockSize = i - startOfBlock;
                    chosenStart = startOfBlock;
                }
            }
            if (!isFree && mem[i] == '-') {
                isFree = true;
                startOfBlock = i;
            }
            if (i == 127 && isFree) {
                if (127 - startOfBlock > blockSize) {
                    blockSize = 127 - startOfBlock;
                    chosenStart = startOfBlock;
                }
            }
        }

        if (blockSize >= proc->size) {
            for (int i = chosenStart; i < (proc->size + chosenStart); i++) {
                mem[i] = proc->pid;
            }
            proc->isLoaded = true;
            proc->numLoads++;
            proc->memIndex = chosenStart;
            numInMem++;
            numLoads++;
            int numHoles = 0;
            bool inHole = false;
            int memUsage = 0;
            
            //Gather stats
            for (int j = 0; j < 128; j++) {
                if (mem[j] != '-') {
                    memUsage++;
                }
                if (!inHole && mem[j] == '-') {
                    numHoles++;
                    inHole = true;
                }
                if (inHole && mem[j] != '-') {
                    inHole = false;
                }
            }
            numInMemAvg += numInMem;
            memUsageAvg += memUsage;
            numHolesAvg += numHoles;
            memUsage = ((100 * memUsage) / 128);

            printAvg = (((100 * memUsageAvg) / 128) / numLoads);

            //Print stats
            printf("%c loaded, #process = %d, #holes = %d, %%memusage = %d, "
                   "cumulative %%mem = %d\n",
                   proc->pid, numInMem, numHoles, memUsage, printAvg);
        }
        
        //If we couldn't load, we need to evict programs until we can
        if (proc->isLoaded == false) {
            if (memQueue->queue[0]->numLoads == 3) {
                swap(memQueue, finishQueue, mem);
            } else {
                swap(memQueue, memQueue, mem);
            }
            numInMem--;
        }
    }

    printAvg = (((100 * memUsageAvg) / 128) / numLoads);
    
    //Print stats
    printf("Total loads = %d, average #processes = %0.1f, average #holes = "
           "%0.1f, cumulative %%mem = %d\n",
           numLoads, (float)((float)numInMemAvg / (float)numLoads),
           (float)((float)numHolesAvg / (float)numLoads), printAvg);
}

void BestFitSim(Queue *memQueue) {
    printf("\nNow simulating using Best Fit memory allocation\n");
    char mem[128];
    int numInMem = 0;
    int numLoads = 0;
    int memUsageAvg = 0;
    int numInMemAvg = 0;
    int numHolesAvg = 0;
    int chosenStart = 0;
    int printAvg = 0;

    Queue *finishQueue = createQueue(memQueue->size);
    for (int i = 0; i < 128; i++) {
        mem[i] = '-';
    }

    while (numInMem != memQueue->size) {
        bool isFree = false;
        Proc *proc = NULL;
        
        //130 is a dummy init value that will be instantly overwritten
        int blockSize = 130;
        int startOfBlock = 0;

        // Find the next process that isn't in memory
        for (int i = 0; i < memQueue->size; i++) {
            if (memQueue->queue[i]->isLoaded == false) {
                proc = memQueue->queue[i];
                break;
            }
        }

        
        for (int i = 0; i < 128; i++) {
            if (isFree && mem[i] != '-') {
                isFree = false;
                
                //If we find a smaller block that works, use it
                if (i - startOfBlock < blockSize &&
                    (i - startOfBlock) >= proc->size) {
                    blockSize = i - startOfBlock;
                    chosenStart = startOfBlock;
                }
            }
            if (!isFree && mem[i] == '-') {
                isFree = true;
                startOfBlock = i;
            }
            
            //End of array case
            if (i == 127 && isFree) {
                if (127 - startOfBlock < blockSize &&
                    (127 - startOfBlock) >= proc->size) {
                    blockSize = 127 - startOfBlock;
                    chosenStart = startOfBlock;
                }
            }
        }
        
        
        if (blockSize != 130 && blockSize >= proc->size) {
            for (int i = chosenStart; i < (proc->size + chosenStart); i++) {
                mem[i] = proc->pid;
            }
            proc->isLoaded = true;
            proc->numLoads++;
            proc->memIndex = chosenStart;
            numInMem++;
            numLoads++;
            int numHoles = 0;
            bool inHole = false;
            int memUsage = 0;
            
            //Collect stats
            for (int j = 0; j < 128; j++) {
                if (mem[j] != '-') {
                    memUsage++;
                }
                if (!inHole && mem[j] == '-') {
                    numHoles++;
                    inHole = true;
                }
                if (inHole && mem[j] != '-') {
                    inHole = false;
                }
            }
            numInMemAvg += numInMem;
            memUsageAvg += memUsage;
            numHolesAvg += numHoles;
            memUsage = ((100 * memUsage) / 128);

            printAvg = (((100 * memUsageAvg) / 128) / numLoads);
            
            //Print stats
            printf("%c loaded, #process = %d, #holes = %d, %%memusage = %d, "
                   "cumulative %%mem = %d\n",
                   proc->pid, numInMem, numHoles, memUsage, printAvg);
        }

        //Evict programs if we couldn't fit into memory
        if (proc->isLoaded == false) {
            if (memQueue->queue[0]->numLoads == 3) {
                swap(memQueue, finishQueue, mem);
            } else {
                swap(memQueue, memQueue, mem);
            }
            numInMem--;
        }
    }

    printAvg = (((100 * memUsageAvg) / 128) / numLoads);
    
    //Print stats
    printf("Total loads = %d, average #processes = %0.1f, average #holes = "
           "%0.1f, cumulative %%mem = %d\n",
           numLoads, (float)((float)numInMemAvg / (float)numLoads),
           (float)((float)numHolesAvg / (float)numLoads), printAvg);
}

void NextFitSim(Queue *memQueue) {
    printf("\nNow simulating using First Fit memory allocation\n");
    char mem[128];
    int numInMem = 0;
    int numLoads = 0;
    int memUsageAvg = 0;
    int numInMemAvg = 0;
    int numHolesAvg = 0;
    int printAvg = 0;
    int startIndex = 0;
    Queue *finishQueue = createQueue(memQueue->size);
    for (int i = 0; i < 128; i++) {
        mem[i] = '-';
    }

    while (numInMem != memQueue->size) {
        bool isFree = false;
        Proc *proc = NULL;
        int blockSize = 0;
        int startOfBlock = 0;

        for (int i = 0; i < memQueue->size; i++) {
            if (memQueue->queue[i]->isLoaded == false) {
                proc = memQueue->queue[i];
                break;
            }
        }

        for (int i = startIndex; i < 128; i++) {
            if (isFree && mem[i] != '-') {
                isFree = false;
                if (i - startOfBlock > blockSize) {
                    blockSize = i - startOfBlock;
                }
            }
            if (!isFree && mem[i] == '-') {
                isFree = true;
                startOfBlock = i;
            }
            if (i == 127 && isFree) {
                blockSize = 127 - startOfBlock;
            }

            if (blockSize >= proc->size) {
                for (int i = startOfBlock; i < (proc->size + startOfBlock);
                     i++) {
                    mem[i] = proc->pid;
                }
                proc->isLoaded = true;
                proc->numLoads++;
                proc->memIndex = startOfBlock;
                numInMem++;
                numLoads++;
                int numHoles = 0;
                bool inHole = false;
                int memUsage = 0;
                
                //Collect stats
                for (int j = 0; j < 128; j++) {
                    if (mem[j] != '-') {
                        memUsage++;
                    }
                    if (!inHole && mem[j] == '-') {
                        numHoles++;
                        inHole = true;
                    }
                    if (inHole && mem[j] != '-') {
                        inHole = false;
                    }
                }
                numInMemAvg += numInMem;
                memUsageAvg += memUsage;
                numHolesAvg += numHoles;
                memUsage = ((100 * memUsage) / 128);
                printAvg = (((100 * memUsageAvg) / 128) / numLoads);
                
                //Print stats
                printf("%c loaded, #process = %d, #holes = %d, %%memusage = "
                       "%d, cumulative %%mem = %d\n",
                       proc->pid, numInMem, numHoles, memUsage, printAvg);
                break;
            }
        }
        
        //Evict programs if we couldn't fit into memory
        if (proc->isLoaded == false) {
            startIndex = 0;
            if (memQueue->queue[0]->numLoads == 3) {
                swap(memQueue, finishQueue, mem);
            } else {
                swap(memQueue, memQueue, mem);
            }
            numInMem--;
        }
    }

    printAvg = (((100 * memUsageAvg) / 128) / numLoads);
    
    //Print stats
    printf("Total loads = %d, average #processes = %0.1f, average #holes = "
           "%0.1f, cumulative %%mem = %d\n",
           numLoads, (float)((float)numInMemAvg / (float)numLoads),
           (float)((float)numHolesAvg / (float)numLoads), printAvg);
}

// Removes element from q1 and adds to q2
void swap(Queue *q1, Queue *q2, char mem[]) {
    Proc *proc = q1->queue[0];

    for (int i = proc->memIndex; i < (proc->size + proc->memIndex); i++) {
        mem[i] = '-';
    }
    proc->isLoaded = false;
    proc->memIndex = 0;
    QueueRemove(q1);
    QueueInsert(q2, proc);
}
