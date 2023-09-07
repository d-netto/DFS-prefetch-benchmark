#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

size_t expected;

typedef struct _node_t {
	struct _node_t *left;
	struct _node_t *right;
	uint32_t v;
	uint8_t set;
} node_t;

static inline void *malloc_s(size_t size)
{
	void *p = malloc(size);
	if (p == NULL) {
		printf("Out of memory\n");
		exit(1);
	}
	return p;
}

node_t *create_node(uint32_t v)
{
	node_t *n = (node_t *)malloc_s(sizeof(node_t));
	n->left = n->right = NULL;
	n->v = v;
	n->set = 0;
	return n;
}

void insert_tree(node_t *root, node_t *node)
{
	if (node->v < root->v) {
		if (root->left == NULL) {
			root->left = node;
		} else {
			insert_tree(root->left, node);
		}
	} else {
		if (root->right == NULL) {
			root->right = node;
		} else {
			insert_tree(root->right, node);
		}
	}
}

node_t *build_binary_tree(size_t nelem)
{
	int j = rand() % nelem;
	node_t *root = create_node(j);
	expected += j;
	for (size_t i = 1; i < nelem; ++i) {
		j = rand() % nelem;
		node_t *node = create_node(j);
		expected += j;
		insert_tree(root, node);
	}
	return root;
}

typedef struct {
	node_t **buffer;
	size_t bottom;
	size_t capacity;
} worklist_t;

worklist_t create_worklist(size_t nelem)
{
	node_t **buffer = (node_t **)malloc_s(sizeof(node_t *) * nelem);
	worklist_t wl = {buffer, 0, nelem};
	return wl;
}

void worklist_push(worklist_t *wl, node_t *node)
{
	wl->buffer[wl->bottom++] = node;
}

node_t *worklist_pop(worklist_t *wl)
{
	node_t *n = NULL;
	if (wl->bottom > 0) {
		n = wl->buffer[--wl->bottom];
	}
	return n;
}

/*
    This is analogous to how the current GC mark-loop
    is implemented in Julia in the sense that we fetch
    the object tag `node->set` before enqueueing it
*/
size_t dfs_eager(node_t *head, size_t nelem, int set)
{
	size_t count = 0;
	worklist_t wl = create_worklist(nelem);
	head->set = set;
	count += head->v;
	worklist_push(&wl, head);
	while (1) {
		node_t *node = worklist_pop(&wl);
		if (node == NULL) {
			break;
		}
		if (node->left != NULL) {
			if (node->left->set != set) {
				node->left->set = set;
				count += node->left->v;
				worklist_push(&wl, node->left);
			}
		}
		if (node->right != NULL) {
			if (node->right->set != set) {
				node->right->set = set;
				count += node->right->v;
				worklist_push(&wl, node->right);
			}
		}
	}
	return count;
}

/*
    This is how the mark-loop could be if we had a single generation
    and didn't have to reconstruct the `remset` on every GC. Note that
    we only fetch the object tag when dequeueing it
*/
size_t dfs_lazy(node_t *head, size_t nelem, int set)
{
	size_t count = 0;
	worklist_t wl = create_worklist(nelem);
	worklist_push(&wl, head);
	while (1) {
		node_t *node = worklist_pop(&wl);
		if (node == NULL) {
			break;
		}
		if (node->set == set) {
			continue;
		}
		node->set = set;
		count += node->v;
		if (node->left != NULL) {
			worklist_push(&wl, node->left);
		}
		if (node->right != NULL) {
			worklist_push(&wl, node->right);
		}
	}
	return count;
}

/*
    Now, let's add a prefetch buffer in front of the mark-stack
*/

#define PREFETCH_BUFFER_CAPACITY 128
#define PREFETCH_BUFFER_WORTH_SIZE 32

typedef struct {
	node_t *buffer[PREFETCH_BUFFER_CAPACITY];
	int32_t top;
	int32_t bottom;
} prefetch_buffer_t;

typedef struct {
	prefetch_buffer_t prefetch_buffer;
	worklist_t worklist;
} prefetch_worklist_t;

prefetch_worklist_t create_prefetch_worklist(size_t nelem)
{
	node_t **buffer = (node_t **)malloc_s(sizeof(node_t *) * nelem);
	worklist_t wl = {buffer, 0, nelem};
	prefetch_worklist_t pfwl;
	pfwl.prefetch_buffer.bottom = pfwl.prefetch_buffer.top = 0;
	pfwl.worklist = wl;
	return pfwl;
}

void prefetch_worklist_push(prefetch_worklist_t *pfwl, node_t *node)
{
	/* Prefetch buffer overflowed */
	if (pfwl->prefetch_buffer.bottom - pfwl->prefetch_buffer.top >=
	    PREFETCH_BUFFER_CAPACITY) {
		pfwl->worklist.buffer[pfwl->worklist.bottom++] = node;
	} else {
		__builtin_prefetch(node);
		pfwl->prefetch_buffer.buffer[pfwl->prefetch_buffer.bottom++ %
					     PREFETCH_BUFFER_CAPACITY] = node;
	}
}

node_t *prefetch_worklist_pop(prefetch_worklist_t *pfwl)
{
	node_t *n = NULL;
	/* Element was prefetched a while back and the memory load may already
	 * be satisfied */
	if (pfwl->prefetch_buffer.bottom - pfwl->prefetch_buffer.top >=
	    PREFETCH_BUFFER_WORTH_SIZE) {
		n = pfwl->prefetch_buffer.buffer[pfwl->prefetch_buffer.top++ %
						 PREFETCH_BUFFER_CAPACITY];
		node_t *n2 = worklist_pop(&pfwl->worklist);
		/* Since we just popped an element, this will push into the
		 * prefetch buffer
		 */
		if (n2 != NULL)
			prefetch_worklist_push(pfwl, n2);
	} else {
		/* Fill up prefetch buffer */
		while (pfwl->prefetch_buffer.bottom -
			   pfwl->prefetch_buffer.top <
		       PREFETCH_BUFFER_WORTH_SIZE) {
			node_t *n2 = worklist_pop(&pfwl->worklist);
			if (n2 == NULL)
				break;
			/* Since `pfwl->prefetch_buffer.bottom -
			   pfwl->prefetch_buffer.top <
			   PREFETCH_BUFFER_WORTH_SIZE`, this will push into the
			   prefetch buffer */
			prefetch_worklist_push(pfwl, n2);
		}
		if (pfwl->prefetch_buffer.bottom > pfwl->prefetch_buffer.top) {
			n = pfwl->prefetch_buffer
				.buffer[pfwl->prefetch_buffer.top++ %
					PREFETCH_BUFFER_CAPACITY];
		}
	}
	return n;
}

size_t dfs_prefetch_eager(node_t *head, size_t nelem, int set)
{
	size_t count = 0;
	prefetch_worklist_t pfwl = create_prefetch_worklist(nelem);
	head->set = set;
	count += head->v;
	prefetch_worklist_push(&pfwl, head);
	while (1) {
		node_t *node = prefetch_worklist_pop(&pfwl);
		if (node == NULL) {
			break;
		}
		if (node->left != NULL) {
			if (node->left->set != set) {
				node->left->set = set;
				count += node->left->v;
				prefetch_worklist_push(&pfwl, node->left);
			}
		}
		if (node->right != NULL) {
			if (node->right->set != set) {
				node->right->set = set;
				count += node->right->v;
				prefetch_worklist_push(&pfwl, node->right);
			}
		}
	}
	return count;
}

size_t dfs_prefetch_lazy(node_t *head, size_t nelem, int set)
{
	size_t count = 0;
	prefetch_worklist_t pfwl = create_prefetch_worklist(nelem);
	prefetch_worklist_push(&pfwl, head);
	while (1) {
		node_t *node = prefetch_worklist_pop(&pfwl);
		if (node == NULL) {
			break;
		}
		if (node->set == set) {
			continue;
		}
		node->set = set;
		count += node->v;
		if (node->left != NULL) {
			prefetch_worklist_push(&pfwl, node->left);
		}
		if (node->right != NULL) {
			prefetch_worklist_push(&pfwl, node->right);
		}
	}
	return count;
}

#define NELEM (1ull << 22)
#define NROUNDS 100

int main(int argc, char **argv)
{
	time_t t;
	srand((unsigned)time(&t));

	int lazy = 0;
	int prefetch = 0;

	if (argc >= 2) {
		lazy |= (strcmp(argv[1], "lazy") == 0);
		prefetch |= (strcmp(argv[1], "prefetch") == 0);
	}
	if (argc == 3) {
		lazy |= (strcmp(argv[2], "lazy") == 0);
		prefetch |= (strcmp(argv[2], "prefetch") == 0);
	}

	if (prefetch && lazy) {
		printf("Running lazy prefetch DFS\n");
		node_t *head = build_binary_tree(NELEM);
		for (int i = 0; i < NROUNDS; ++i) {
			printf("iter=%d\n", i);
			size_t count =
			    dfs_prefetch_lazy(head, NELEM, i % 2 == 0);
			assert(count == expected);
		}
	} else if (lazy) {
		printf("Running lazy DFS\n");
		node_t *head = build_binary_tree(NELEM);
		for (int i = 0; i < NROUNDS; ++i) {
			printf("iter=%d\n", i);
			size_t count = dfs_lazy(head, NELEM, i % 2 == 0);
			assert(count == expected);
		}
	} else if (prefetch) {
		printf("Running eager prefetch DFS\n");
		node_t *head = build_binary_tree(NELEM);
		for (int i = 0; i < NROUNDS; ++i) {
			printf("iter=%d\n", i);
			size_t count =
			    dfs_prefetch_eager(head, NELEM, i % 2 == 0);
			assert(count == expected);
		}
	} else {
		printf("Running eager DFS\n");
		node_t *head = build_binary_tree(NELEM);
		for (int i = 0; i < NROUNDS; ++i) {
			printf("iter=%d\n", i);
			size_t count = dfs_eager(head, NELEM, i % 2 == 0);
			assert(count == expected);
		}
	}
}