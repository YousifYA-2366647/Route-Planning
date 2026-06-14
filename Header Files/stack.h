#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct StackElement {
    float key;
    int value;
} StackElement;

typedef struct CustomStack {
    StackElement** elements;
    int* pos;
    int maxSize;
    int curSize;
} CustomStack;

StackElement* create_stack_element(float key, int value);

CustomStack* create_stack(int maxSize);

void insert_stack(CustomStack* stack, float key, int value);

StackElement* pop_top_stack(CustomStack* stack);

bool element_in_stack(CustomStack* stack, int value);

bool stack_empty(CustomStack* stack);

void free_stack(CustomStack* stack);

void print_stack(CustomStack* stack);

#endif