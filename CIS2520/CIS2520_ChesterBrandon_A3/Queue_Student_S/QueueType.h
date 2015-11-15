#include "StudentInterface.h"
typedef Student Item;

#define MAXQUEUESIZE 4
typedef struct {
	Item items[MAXQUEUESIZE];
	int size;
    int head;
} Queue;
