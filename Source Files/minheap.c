#ifndef MINHEAP_C
#define MINHEAP_C

#include "../Header Files/minheap.h"

Minheap* create_minheap(int maxSize) {
    Minheap* heap = malloc(sizeof(Minheap));

    heap->curSize = 0;
    heap->maxSize = maxSize - 1;
    heap->pos = malloc(maxSize * sizeof(int));
    for (int i = 0; i < maxSize; i++) {
        heap->pos[i] = -1;
    }
    heap->elements = malloc(maxSize * sizeof(HeapNode*));

    return heap;
}

double peek_min_key(Minheap* heap) {
    if (heap->curSize != 0) {
        return heap->elements[0]->key;
    }
    return -1.0;
}

int peek_min_value(Minheap* heap) {
    if (heap->curSize != 0) {
        return heap->elements[0]->value;
    }
    return -1;
}

HeapNode* extract_min_minheap(Minheap* heap) {
    if (heap->curSize == 0) {
        return NULL;
    }

    HeapNode* root = heap->elements[0];

    heap->elements[0] = heap->elements[heap->curSize-1];
    heap->pos[root->value] = -1;
    heap->pos[heap->elements[0]->value] = 0;
    heap->curSize--;

    if (heap->curSize < 0) heap->curSize = 0;

    heapify(heap, 0);

    return root;
}

double get_key_minheap(Minheap* heap, int value) {
    return heap->elements[heap->pos[value]]->key;
}

bool element_exists_minheap(Minheap* heap, int value) {
    return heap->pos[value] != -1;
}

void swap_elements(Minheap* heap, int from, int to) {
    heap->pos[heap->elements[from]->value] = to;
    heap->pos[heap->elements[to]->value] = from;

    HeapNode* temp = heap->elements[from];
    heap->elements[from] = heap->elements[to];
    heap->elements[to] = temp;
}

void heapify(Minheap* heap, int element) {
    int smallest = element;
    int left = 2 * element + 1;
    int right = 2 * element + 2;

    if (left < heap->curSize && heap->elements[left]->key < heap->elements[smallest]->key) {
        smallest = left;
    }

    if (right < heap->curSize && heap->elements[right]->key < heap->elements[smallest]->key) {
        smallest = right;
    }

    if (smallest != element) {
        swap_elements(heap, element, smallest);
        heapify(heap, smallest);
    }
}

void insert_element_minheap(Minheap* heap, double key, int value) {
    if (heap->curSize > heap->maxSize) {
        return;
    }

    HeapNode* newNode = malloc(sizeof(HeapNode));
    newNode->key = key;
    newNode->value = value;

    heap->elements[heap->curSize] = newNode;
    heap->pos[value] = heap->curSize;
    heap->curSize++;

    int index = heap->curSize - 1;
    while (index > 0 && heap->elements[(index - 1) / 2]->key > heap->elements[index]->key) {
        swap_elements(heap, index, (index-1)/2);

        index = (index-1)/2;
    }
}

void delete_element(Minheap* heap, int value) {
    decrease_key_minheap(heap, value, -1);
    extract_min_minheap(heap);
}

void decrease_key_minheap(Minheap* heap, int value, double newKey) {
    if (heap->pos[value] == -1) {
        return;
    }

    heap->elements[heap->pos[value]]->key = newKey;

    int index = heap->pos[value];
    int parentIndex = (index-1)/2;
    while (index > 0 && heap->elements[parentIndex]->key > heap->elements[index]->key) {
        swap_elements(heap, index, parentIndex);

        index = parentIndex;
        parentIndex = (index - 1)/2;
    }
}

bool minheap_empty(Minheap* heap) {
    return heap->curSize == 0;
}

void free_minheap(Minheap* heap) {
    for (int i = 0; i < heap->curSize; i++) {
        free(heap->elements[i]);
    }
    free(heap->elements);
    free(heap->pos);
    free(heap);
}

void print_minheap(Minheap* heap) {
    printf("[");
    for (int i = 0; i < heap->curSize; i++) {
        printf("(%d; %f)", heap->elements[i]->value, heap->elements[i]->key);
        if (i < heap->curSize - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

#endif