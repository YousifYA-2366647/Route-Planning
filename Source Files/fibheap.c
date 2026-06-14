#ifndef FIBHEAP_C
#define FIBHEAP_C

#define PHI 1.61803398875

#include "../Header Files/fibheap.h"
#include <math.h>

// ========================================================================================= //
//                                   Helper Functions                                        //
// ========================================================================================= //

// Returns the maximum degree a node in the fibonacci heap can have
static int max_degree(int n) {
    if (n <= 1) return 0;
    return (int)(ceil(log((double)n) / log(PHI))) + 1;
}

// Adds a node to the root list (or a child list) of a fibonacci heap
static void add_to_list(FibNode* list, FibNode* node) {
    if (!node || !list) return;
    node->right = list;
    node->left = list->left;
    list->left->right = node;
    list->left = node;
}

// Removes a node from the list it's currently in
static void remove_from_list(FibNode* node) {
    if (!node) return;
    node->left->right = node->right;
    node->right->left = node->left;
    node->left = node;
    node->right = node;
}

// Links two nodes as parent and child
static void link_nodes(Fibheap* heap, FibNode* parent, FibNode* child) {
    remove_from_list(child);
    if (!parent->child) {
        parent->child = child;
        child->left = child;
        child->right = child;
    }
    else {
        add_to_list(parent->child, child);
    }

    child->parent = parent;
    parent->degree++;
    child->isMarked = false;
}

// Cuts a child from its parent in a fibonacci heap
static void cut(Fibheap* heap, FibNode* parent, FibNode* child) {
    if (parent->child == child) {
        if (child->right == child) {
            parent->child = NULL;
        }
        else {
            parent->child = child->right;
        }
    }
    remove_from_list(child);
    parent->degree--;

    add_to_list(heap->minRoot, child);
    child->parent = NULL;
    child->isMarked = false;
}

// A marked child is cut from its parent and added to the root list.
// then the parent is considered, until a node is reached that is unmarked
static void cascading_cut(Fibheap* heap, FibNode* node) {
    FibNode* parent = node->parent;
    if (parent) {
        if (!node->isMarked) {
            node->isMarked = true;
        }
        else {
            cut(heap, parent, node);
            cascading_cut(heap, parent);
        }
    }
}

// Frees the node lists in a fibonacci heap
static void free_node_recursively(FibNode* node) {
    if (!node) return;

    if (node->child) {
        FibNode* child = node->child;
        FibNode* firstChild = child;
        do {
            FibNode* nextChild = child->right;
            free_node_recursively(child);
            child = nextChild;
        } while (child != firstChild);
    }
    free(node);
}

// Prints the node list in a fibonacci heap
static void print_node_list(FibNode* start, int depth) {
    if (!start) return;

    FibNode* cur = start;
    do {
        for (int i = 0; i < depth; i++) {
            printf("    ");
        }

        printf("%d\n", cur->value);

        if (cur->child) {
            print_node_list(cur->child, depth + 1);
        }

        cur = cur->right;
    } while (cur != start);
}

// ========================================================================================= //
//                                   Core Operations                                         //
// ========================================================================================= //

FibNode* create_fib_node(float key, int value) {
    FibNode* node = malloc(sizeof(FibNode));

    node->child = NULL;
    node->parent = NULL;
    node->left = node;
    node->right = node;
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->isMarked = false;

    return node;
}

Fibheap* create_fibheap() {
    Fibheap* heap = malloc(sizeof(Fibheap));

    heap->nodeCount = 0;
    heap->minRoot = NULL;

    return heap;
}

Fibheap* merge(Fibheap* heap1, Fibheap* heap2) {
    if (!heap1 && !heap2) return NULL;
    if (!heap1 || !heap1->minRoot) return heap2;
    if (!heap2 || !heap2->minRoot) return heap1;

    Fibheap* newHeap = create_fibheap();
    newHeap->nodeCount = heap1->nodeCount + heap2->nodeCount;
    newHeap->minRoot = heap1->minRoot;

    FibNode* heap1Last = heap1->minRoot->left;
    FibNode* heap2Last = heap2->minRoot->left;

    heap1Last->right = heap2->minRoot;
    heap2->minRoot->left = heap1Last;
    heap2Last->right = heap1->minRoot;
    heap1->minRoot->left = heap2Last;

    if (heap2->minRoot->key <= newHeap->minRoot->key) {
        newHeap->minRoot = heap2->minRoot;
    }

    return newHeap;
}

FibNode* fib_insert_element(Fibheap* heap, float key, int value) {
    if (!heap) return NULL;

    FibNode* newNode = create_fib_node(key, value);

    if (!heap->minRoot) {
        heap->minRoot = newNode;
    }
    else {
        add_to_list(heap->minRoot, newNode);
        if (newNode->key < heap->minRoot->key) {
            heap->minRoot = newNode;
        }
    }
    heap->nodeCount++;

    return newNode;
}

FibNode* fib_extract_min(Fibheap* heap) {
    if (!heap || !heap->minRoot) return NULL;

    FibNode* min = heap->minRoot;
    if (min->child) {
        FibNode* child = min->child;
        FibNode* firstChild = child;

        do {
            FibNode* nextChild = child->right;
            child->parent = NULL;

            if (min->right == min) {
                heap->minRoot = child;
            }
            else {
                add_to_list(heap->minRoot, child);
            }
            child = nextChild;
        } while (child != firstChild);

        heap->minRoot = firstChild;
    }
    else {
        if (min->right == min) {
            heap->minRoot = NULL;
        }
        else {
            heap->minRoot = min->right;
        }
    }

    remove_from_list(min);
    if (heap->minRoot) fib_consolidate(heap);
    heap->nodeCount--;

    min->left = min;
    min->right = min;
    min->parent = NULL;
    min->child = NULL;

    return min;
}

bool fib_empty_heap(Fibheap* heap) {
    return (!heap || !heap->minRoot);
}

void fib_decrease_key(Fibheap* heap, FibNode* node, float newValue) {
    if (!heap || !node || newValue > node->key) return;

    node->key = newValue;
    FibNode* parent = node->parent;

    if (parent && node->key <= parent->key) {
        cut(heap, parent, node);
        cascading_cut(heap, parent);
    }

    if (node->key < heap->minRoot->key) {
        heap->minRoot = node;
    }
}

void fib_delete_node(Fibheap* heap, FibNode* node) {
    if (!heap || !node) return;

    fib_decrease_key(heap, node, -INFINITY);
    FibNode* extracted = fib_extract_min(heap);
    if (extracted) free(extracted);
}

void fib_consolidate(Fibheap* heap) {
    if (!heap || !heap->minRoot) return;

    int buffer = 7;
    int maxDeg = max_degree(heap->nodeCount) + buffer;
    FibNode** degrees = calloc(maxDeg, sizeof(FibNode*));
    
    int rootCount = 0;
    FibNode* current = heap->minRoot;
    do {
        rootCount++;
        current = current->right;
    } while (current != heap->minRoot);
    
    FibNode** roots = malloc(rootCount * sizeof(FibNode*));
    current = heap->minRoot;
    for (int i = 0; i < rootCount; i++) {
        roots[i] = current;
        current = current->right;
    }

    for (int i = 0; i < rootCount; i++) {
        FibNode* node = roots[i];
        int degree = node->degree;
        while (degrees[degree]) {
            FibNode* other = degrees[degree];

            if (other->key < node->key) {
                FibNode* temp = node;
                node = other;
                other = temp;
            }

            link_nodes(heap, node, other);
            heap->minRoot = node;
            degrees[degree] = NULL;
            degree++;
        }
        degrees[degree] = node;
    }

    heap->minRoot = NULL;
    for (int i = 0; i < maxDeg; i++) {
        if (degrees[i]) {
            if (!heap->minRoot) {
                heap->minRoot = degrees[i];
                heap->minRoot->left = heap->minRoot;
                heap->minRoot->right = heap->minRoot;
            }
            else {
                add_to_list(heap->minRoot, degrees[i]);
                if (degrees[i]->key < heap->minRoot->key) {
                    heap->minRoot = degrees[i];
                }
            }
        }
    }

    free(degrees);
    free(roots);
}

void fib_free_heap(Fibheap* heap) {
    if (!heap) return;

    if (heap->minRoot) {
        FibNode* root = heap->minRoot;
        FibNode* firstRoot = root;
        do {
            FibNode* nextRoot = root->right;
            free_node_recursively(root);
            root = nextRoot;
        } while (root != firstRoot);
    }
    free(heap);
}

void fib_print_heap(Fibheap* heap) {
    if (!heap || !heap->minRoot) return;
    print_node_list(heap->minRoot, 0);
}

#endif