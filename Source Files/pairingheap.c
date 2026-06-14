#ifndef PAIRINGHEAP_C
#define PAIRINGHEAP_C

#include "../Header Files/pairingheap.h"

PairingHeap* create_pairingheap() {
    PairingHeap* heap = malloc(sizeof(PairingHeap));
    heap->root = NULL;
    return heap;
}

PairingHeapNode* find_min_pairing(PairingHeap* heap) {
    return heap->root;
}

PairingHeap* meld(PairingHeap* heap1, PairingHeap* heap2) {
    if (empty_pairingheap(heap1)) return heap2;
    if (empty_pairingheap(heap2)) return heap1;
    if (heap1->root->key > heap2->root->key) {
        heap1->root->next = heap2->root->child;
        if (heap2->root->child) {
            heap2->root->child->prev = heap1->root;
        }
        heap2->root->child = heap1->root;
        heap1->root->prev = heap2->root;
        heap2->size += heap1->size;
        return heap2;
    }
    else {
        heap2->root->next = heap1->root->child;
        if (heap1->root->child) {
            heap1->root->child->prev = heap2->root;
        }
        heap1->root->child = heap2->root;
        heap2->root->prev = heap1->root;
        heap1->size += heap2->size;
        return heap1;
    }
}

PairingHeapNode* meld_nodes(PairingHeapNode* node1, PairingHeapNode* node2) {
    if (!node1) return node2;
    if (!node2) return node1;
    if (node1 == node2) return node1;

    if (node1->key < node2->key) {
        node2->next = node1->child;
        if (node1->child) {
            node1->child->prev = node2;
        }
        node2->prev = node1;
        node1->child = node2;
        return node1;
    }
    else {
        node1->next = node2->child;
        if (node2->child) {
            node2->child->prev = node1;
        }
        node1->prev = node2;
        node2->child = node1;
        return node2;
    }
}

PairingHeapNode* insert_pairingheap_element(PairingHeap* heap, float key, int value) {
    PairingHeapNode* newNode = malloc(sizeof(PairingHeapNode));
    newNode->value = value;
    newNode->key = key;
    newNode->child = NULL;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (!heap->root) {
        heap->root = newNode;
    }
    else {
        heap->root = meld_nodes(heap->root, newNode);
    }

    return newNode;
}

void decrease_key_pairingheap(PairingHeap* heap, PairingHeapNode* node, float newValue) {
    if (newValue > node->key) return;
    node->key = newValue;

    if (node == heap->root) return;
    if (node->next) {
        node->next->prev = node->prev;
    }
    if (node->prev->child == node) {
        node->prev->child = node->next;
    }
    else {
        node->prev->next = node->next;
    }

    node->next = NULL;
    node->prev = NULL;

    heap->root = meld_nodes(heap->root, node);
}

PairingHeapNode* delete_min_pairing(PairingHeap* heap) {
    if (!heap->root) return NULL;

    PairingHeapNode* oldRoot = heap->root;
    heap->root = NULL;
    PairingHeapNode* firstChild = oldRoot->child;

    if (!firstChild) {
        heap->root = NULL;
    } else {
        int capacity = 64;
        int count = 0;
        PairingHeapNode** children = malloc(capacity * sizeof(PairingHeapNode*));
        PairingHeapNode* current = oldRoot->child;

        while (current) {
            if (count >= capacity) {
                capacity *= 2;
                children = realloc(children, capacity * sizeof(PairingHeapNode*));
            }
            
            PairingHeapNode* nextSib = current->next;

            current->next = NULL;
            current->prev = NULL; 
            
            children[count++] = current;
            current = nextSib;
        }

        for (size_t i = 0; i + 1 < count; i += 2) {
            children[i] = meld_nodes(children[i], children[i + 1]);
        }

        int lastIndex = count - 1;
        if (count % 2 != 0) {
            lastIndex = count - 1;
        } else {
            lastIndex = count - 2;
        }

        PairingHeapNode* newRoot = children[lastIndex];
        for (int i = lastIndex - 2; i >= 0; i -= 2) {
            newRoot = meld_nodes(children[i], newRoot);
        }

        heap->root = newRoot;

        free(children);
    }

    return oldRoot;
}

bool empty_pairingheap(PairingHeap* heap) {
    return heap->root == NULL;
}

void print_nodes(PairingHeapNode* node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("|-- Key: %.2f (Val: %d)\n", node->key, node->value);

    if (node->child) {
        print_nodes(node->child, depth + 1);
    }

    if (node->next) {
        print_nodes(node->next, depth);
    }
}

void print_pairingheap(PairingHeap* heap) {
    if (heap == NULL || heap->root == NULL) {
        printf("Heap is empty.\n");
        return;
    }
    printf("Pairing Heap Structure:\n");
    print_nodes(heap->root, 0);
}

void free_nodes(PairingHeapNode* node) {
    if (node == NULL) return;

    free_nodes(node->child);
    free_nodes(node->next);

    free(node);
}

void free_pairingheap(PairingHeap* heap) {
    if (heap == NULL) return;
    
    free_nodes(heap->root);
    
    free(heap);
}


#endif