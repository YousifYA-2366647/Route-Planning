#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct QueueElement {
    float key;
    int value;
} QueueElement;

typedef struct Queue {
    QueueElement** elements;
    int* pos;
    int curFirst;
    int curSize;
    int maxSize;
} Queue;

Queue* create_queue(int maxSize);

QueueElement* create_queue_element(float key, int value);

void insert_queue(Queue* queue, float key, int value);

QueueElement* pop_queue(Queue* queue);

bool element_in_queue(Queue* queue, int value);

bool queue_empty(Queue* queue);

void free_queue(Queue* queue);

void print_queue(Queue* queue);

#endif