#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// An element in the stack with a key and a vlaue
typedef struct StackElement {
    float key;
    int value;
} StackElement;

// A stack containing elements, uses a FCLS/LCFS approach
typedef struct CustomStack {
    StackElement** elements;
    int* pos;
    int maxSize;
    int curSize;
} CustomStack;

// creates an element for the stack with the given key and value
StackElement* create_stack_element(float key, int value);

// Creates an empty stack with a maximum capacity
CustomStack* create_stack(int maxSize);

// Inserts an element into the stack
void insert_stack(CustomStack* stack, float key, int value);

// Pops the top element in the stack
StackElement* pop_top_stack(CustomStack* stack);

// Checks whether an element is in the stack
bool element_in_stack(CustomStack* stack, int value);

// Checks whether the stack is empty
bool stack_empty(CustomStack* stack);

// Frees the resources used by a stack
void free_stack(CustomStack* stack);

// Prints the stack
void print_stack(CustomStack* stack);

#endif