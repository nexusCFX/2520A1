 
typedef struct TreeNodeTag {
	void *value;
	struct TreeNodeTag *left, *right, *parent;
} TreeNode;

typedef struct {
	TreeNode *root;
	TreeNode *current;
	void * (*copyValue) (void *, void *);
	void (*destroyValue) (void *);
	int (*compareValues) (void *, void *);
	int size;
} Tree;



