#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _node_t {
    struct _node_t *left;
    struct _node_t *right;
    uint64_t v;
    uint8_t set;
} node_t;

node_t *create_node(size_t v)
{
    node_t *n = (node_t *)malloc(sizeof(node_t));
    n->left = n->right = NULL;
    n->v = v;
    n->set = 0;
    return n;
}

void insert_tree(node_t *root, node_t *node)
{
    if (root->v <= node->v) {
        if (root->right == NULL) {
            root->right = node;
        }
        else {
            insert_tree(root->right, node);
        }
    }
    else {
        if (root->left == NULL) {
            node->left = node;
        }
        else {
            insert_tree(root->left, node);
        }
    }
}

node_t *build_binary_tree(size_t nelem)
{
    node_t *root = create_node(0);
    for (size_t i = 1; i < nelem; ++i) {
        node_t *node = create_node(i);
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
    node_t **buffer = (node_t **)malloc(sizeof(node_t *) * nelem);
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
    if (wl->bottom > 0)
        n = wl->buffer[--wl->bottom];
    return n;
}

/*
    This is analogous to how the current GC mark-loop
    is implemented in Julia in the sense that we fetch
    the object tag `node->set` before enqueueing it
*/
size_t dfs_eager(node_t *head, size_t nelem)
{
    size_t count = 0;
    worklist_t wl = create_worklist(nelem);
    head->set = 1;
    count += head->v;
    worklist_push(&wl, head);
    while (1) {
        node_t *node = worklist_pop(&wl);
        if (node == NULL) {
            break;
        }
        if (node->left != NULL) {
            if (!node->left->set) {
                node->left->set = 1;
                count += node->left->v;
                worklist_push(&wl, node->left);
            }
        }
        if (node->right != NULL) {
            if (!node->right->set) {
                node->right->set = 1;
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
size_t dfs_lazy(node_t *head, size_t nelem)
{
    size_t count = 0;
    worklist_t wl = create_worklist(nelem);
    worklist_push(&wl, head);
    while (1) {
        node_t *node = worklist_pop(&wl);
        if (node == NULL) {
            break;
        }
        if (node->set) {
            continue;
        }
        node->set = 1;
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

#define NELEM (1ull << 16)

int main(int argc, char **argv)
{
    size_t expected = (NELEM * (NELEM - 1)) / 2;

    /* Eager DFS */
    node_t *head = build_binary_tree(NELEM);
    size_t count1 = dfs_eager(head, NELEM);
    assert(count1 == expected);

    /* Lazy DFS */
    head = build_binary_tree(NELEM);
    size_t count2 = dfs_lazy(head, NELEM);
    assert(count2 == expected);
    return 0;
}