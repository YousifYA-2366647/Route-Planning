#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
    A node in the heap.

    key: a value that is used to compare nodes
    value: a unique value to differentiate nodes
*/
typedef struct HeapNode {
    double key;
    int value;
} HeapNode;

/*
    A heap where a node's children are always smaller than their parent.
    The root is the smallest node.

    curSize: the size of the heap
    maxSize: the capacity of the heap.
    elements: a list of elements in the heap.
    pos: a list to save where certain elements are in the 'elements' array
*/
typedef struct Minheap {
    int curSize;
    int maxSize;
    HeapNode** elements;
    int* pos;
} Minheap;

// Creates and returns a minheap with the given capacity
Minheap* create_minheap(int maxSize);

// Checks whether a certain element exists in the heap
bool element_exists_minheap(Minheap* heap, int value);

// Gets the key value of a certain element in the heap
double get_key_minheap(Minheap* heap, int value);

// Returns the key of the root of the heap
double peek_min_key(Minheap* heap);

// Returns the value of the root of the heap
int peek_min_value(Minheap* heap);

// Extracts and returns the root
HeapNode* extract_min_minheap(Minheap* heap);

// Maintains the heap-structure in the 'elements' array
void heapify(Minheap* heap, int element);

// Swaps two elements in the heap
void swap_elements(Minheap* heap, int element1, int element2);

// Changes the key of an elements with the given value
void decrease_key_minheap(Minheap* heap, int value, double newKey);

// Inserts a new element into the heap
void insert_element_minheap(Minheap* heap, double key, int value);

// Deletes the given element from the heap
void delete_element(Minheap* heap, int value);

// Prints the heap
void print_minheap(Minheap* heap);

// Checks whether the heap is empty
bool minheap_empty(Minheap* heap);

// Frees the resources used by the heap
void free_minheap(Minheap* heap);

#endif