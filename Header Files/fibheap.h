#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/*
    A node in the fibonacci heap.
    key: this is compared when choosing who becomes the parent and root.
    value: a label to recognise nodes, this key is unique to the node.
    isMarked: when a node loses a child, it is marked. If it loses two children, it is severed from its parent and it becomes the root of a new tree.
    degree: the amount of children the node has.
    parent, left, right, child: the connected nodes.
*/
typedef struct FibNode {
    float key;
    int value;
    bool isMarked;
    int degree;
    struct FibNode* parent;
    struct FibNode* left;
    struct FibNode* right;
    struct FibNode* child;
} FibNode;

/*
    The fibonacci heap
    nodeCount: the amount of nodes currently in the heap.
    minRoot: a pointer to the root, the smallest element in the heap.
*/
typedef struct Fibheap {
    int nodeCount;
    FibNode* minRoot;
} Fibheap;

// Creates a node for the fibonacci heap with the given key and value.
FibNode* create_fib_node(float key, int value);

// Creates an empty fibonacci heap
Fibheap* create_fibheap();

// Merges two given fibonacci heaps and returns the new heap
Fibheap* merge(Fibheap* heap1, Fibheap* heap2);

// Inserts a new node with the given key and element into the heap and returns the new node
FibNode* fib_insert_element(Fibheap* heap, float key, int value);

// Returns the root node of a fibonacci heap
FibNode* fib_extract_min(Fibheap* heap);

// Checks whether the given heap is empty
bool fib_empty_heap(Fibheap* heap);

// Decreases the value of an existing in node in the heap
void fib_decrease_key(Fibheap* heap, FibNode* node, float newValue);

// Deletes a node from the heap
void fib_delete_node(Fibheap* heap, FibNode* node);

// Retains the structure of the fibonacci heap by making nodes with the same degree the child of the other
void fib_consolidate(Fibheap* heap);

// Frees the resources used by a fibonacci heap
void fib_free_heap(Fibheap* heap);

// Prints the fibonacci heap
void fib_print_heap(Fibheap* heap);

#endif