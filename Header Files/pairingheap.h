#ifndef PAIRINGHEAP_H
#define PAIRINGHEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct PairingHeapNode {
    int value;
    float key;
    struct PairingHeapNode* next;
    struct PairingHeapNode* prev;
    struct PairingHeapNode* child;
} PairingHeapNode;

typedef struct PairingHeap {
    PairingHeapNode* root;
    int size;
} PairingHeap;

PairingHeap* create_pairingheap();

PairingHeapNode* find_min_pairing(PairingHeap* heap);

PairingHeap* meld(PairingHeap* heap1, PairingHeap* heap2);

PairingHeapNode* meld_nodes(PairingHeapNode* node1, PairingHeapNode* node2);

PairingHeapNode* insert_pairingheap_element(PairingHeap* heap, float key, int value);

void decrease_key_pairingheap(PairingHeap* heap, PairingHeapNode* node, float newValue);

PairingHeapNode* delete_min_pairing(PairingHeap* heap);

bool empty_pairingheap(PairingHeap* heap);

void print_pairingheap(PairingHeap* heap);

void free_pairingheap(PairingHeap* heap);

#endif