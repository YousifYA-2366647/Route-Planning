#ifndef PAIRINGHEAP_H
#define PAIRINGHEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// A node in the pairing heap
// - value: a stored value
// - key: the key that the nodes are compared by
// - next: right sibling
// - prev: left sibling/parent
// - child: child node
typedef struct PairingHeapNode {
    int value;
    float key;
    struct PairingHeapNode* next;
    struct PairingHeapNode* prev;
    struct PairingHeapNode* child;
} PairingHeapNode;

// A pairing heap, only keeps track of the root and the size of the heap
typedef struct PairingHeap {
    PairingHeapNode* root;
    int size;
} PairingHeap;

// Creates an empty pairing heap
PairingHeap* create_pairingheap();

// Returns the root without deleting it from the heap
PairingHeapNode* find_min_pairing(PairingHeap* heap);

// Merges two pairing heaps and returns the result
PairingHeap* meld(PairingHeap* heap1, PairingHeap* heap2);

// Merges two nodes in a heap by making the node with the greates key the child of the other
PairingHeapNode* meld_nodes(PairingHeapNode* node1, PairingHeapNode* node2);

// Inserts a new node into the heap with the given key and value
PairingHeapNode* insert_pairingheap_element(PairingHeap* heap, float key, int value);

// Decreases the key of the given node in the heap
void decrease_key_pairingheap(PairingHeap* heap, PairingHeapNode* node, float newValue);

// Extracts the root from a heap
PairingHeapNode* delete_min_pairing(PairingHeap* heap);

// Checks whether the pairing heap is empty
bool empty_pairingheap(PairingHeap* heap);

// Prints the pairing heap
void print_pairingheap(PairingHeap* heap);

// Frees used resources of the pairing heap
void free_pairingheap(PairingHeap* heap);

#endif