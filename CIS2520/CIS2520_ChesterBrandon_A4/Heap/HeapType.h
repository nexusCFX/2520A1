

typedef struct {
	void **heap;
	int capacity;
	int numberOfValues;
	void * (*copyValue) (void *, void *);
	void (*destroyValue) (void *);
	int (*compareValues) (void *, void *);
} Heap;



