#ifndef STACK_C
#define STACK_C

#include "../Header Files/stack.h"

StackElement* create_stack_element(float key, int value) {
    StackElement* element = malloc(sizeof(StackElement));
    element->key = key;
    element->value = value;

    return element;
}

CustomStack* create_stack(int maxSize) {
    CustomStack* stack = malloc(sizeof(CustomStack));

    stack->curSize = 0;
    stack->maxSize = maxSize;
    stack->elements = calloc(maxSize, sizeof(StackElement*));
    stack->pos = malloc(maxSize * sizeof(int));

    for (int i = 0; i < maxSize; i++) {
        stack->pos[i] = -1;
    }

    return stack;
}

void insert_stack(CustomStack* stack, float key, int value) {
    if (stack->curSize == stack->maxSize) return;
    StackElement* newElem = create_stack_element(key, value);

    stack->elements[stack->curSize++] = newElem;
    stack->pos[value] = stack->curSize - 1;
}

StackElement* pop_top_stack(CustomStack* stack) {
    if (stack->curSize == 0) return NULL;
    StackElement* top = stack->elements[stack->curSize - 1];
    stack->curSize--;
    stack->elements[stack->curSize] = NULL;
    stack->pos[top->value] = -1;

    return top;
}

bool element_in_stack(CustomStack* stack, int value) {
    return stack->pos[value] != -1;
}

bool stack_empty(CustomStack* stack) {
    return stack->curSize == 0;
}

void free_stack(CustomStack* stack) {
    for (int i = 0; i < stack->curSize; i++) {
        free(stack->elements[i]);
    }
    free(stack->elements);
    free(stack->pos);
    free(stack);
}

void print_stack(CustomStack* stack) {
    printf("[");
    for (int i = 0; i < stack->curSize; i++) {
        printf("(%d;%f)", stack->elements[i]->value, stack->elements[i]->key);
        if (i != stack->curSize - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

#endif