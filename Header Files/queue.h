#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// An element in the Queue, has a key and a value
typedef struct QueueElement {
    float key;
    int value;
} QueueElement;

// A queue with a FCFS approach
typedef struct Queue {
    QueueElement** elements;
    int* pos;
    int curFirst;
    int curSize;
    int maxSize;
} Queue;

// Creates an empty queue with a maximum capacity
Queue* create_queue(int maxSize);

// Creates an element to be inserted in the queue
QueueElement* create_queue_element(float key, int value);

// Inserts an element into the queue
void insert_queue(Queue* queue, float key, int value);

// Returns the first element in the queue
QueueElement* pop_queue(Queue* queue);

// Checks whether a certain element is in the queue
bool element_in_queue(Queue* queue, int value);

// Checks whether the queue is empty
bool queue_empty(Queue* queue);

// Frees the resources used by the queue
void free_queue(Queue* queue);

// Prints the queue
void print_queue(Queue* queue);

#endif