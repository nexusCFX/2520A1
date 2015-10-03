#include "StudentInterface.h"
typedef Student Item;

#define MAXLISTSIZE 4
typedef struct {
	Item items[MAXLISTSIZE];
	int size;
} List;
