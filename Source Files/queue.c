#ifndef QUEUE_C
#define QUEUE_C

#include "../Header Files/queue.h"

Queue* create_queue(int maxSize) {
    Queue* queue = malloc(sizeof(Queue));
    queue->elements = calloc(maxSize, sizeof(QueueElement*));
    queue->pos = malloc(maxSize * sizeof(int));

    for (int i = 0; i < maxSize; i++) {
        queue->pos[i] = -1;
    }

    queue->curFirst = 0;
    queue->curSize = 0;
    queue->maxSize = maxSize;

    return queue;
}

QueueElement* create_queue_element(float key, int value) {
    QueueElement* elem = malloc(sizeof(QueueElement));

    elem->key = key;
    elem->value = value;

    return elem;
}

void insert_queue(Queue* queue, float key, int value) {
    QueueElement* newElement = create_queue_element(key, value);

    queue->elements[queue->curSize] = newElement;
    queue->pos[newElement->value] = queue->curSize;
    queue->curSize++;
    if (queue->curSize == queue->maxSize) {
        queue->curSize = 0;
    }
}

QueueElement* pop_queue(Queue* queue) {
    if (queue->curFirst == queue->curSize) {
        return NULL;
    }
    QueueElement* min = queue->elements[queue->curFirst];
    queue->elements[queue->curFirst] = NULL;
    queue->pos[min->value] = -1;
    queue->curFirst++;

    return min;
}

bool element_in_queue(Queue* queue, int value) {
    return queue->pos[value] != -1;
}

bool queue_empty(Queue* queue) {
    return queue->curSize == queue->curFirst;
}

void free_queue(Queue* queue) {
    for (int i = 0; i < queue->maxSize; i++) {
        if (queue->elements[i]) {
            free(queue->elements[i]);
        }
    }
    free(queue->elements);
    free(queue);
}

void print_queue(Queue* queue) {
    printf("[");
    for (int i = 0; i < queue->maxSize; i++) {
        if (queue->elements[i]) {
            printf("(%d;%f)", queue->elements[i]->value, queue->elements[i]->key);
        }
        if (i != queue->maxSize - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

#endif