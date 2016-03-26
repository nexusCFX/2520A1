#include "ProcLib.h"

Proc* createProcess(int size, char pid) {
    Proc* proc = malloc(sizeof(*proc));
    proc->size = size;
    proc->memIndex = 0;
    proc->pid = pid;
    proc->numLoads = 0;
    proc->isLoaded = false;
    return proc;
}
