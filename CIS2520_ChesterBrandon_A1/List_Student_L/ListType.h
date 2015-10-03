#include "StudentInterface.h"
typedef Student Item;

typedef struct Node {
    Student item;
    struct Node* next;
} Node;

typedef struct {
    Node* root;
    int size;
} List;
