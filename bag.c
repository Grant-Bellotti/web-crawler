#include <stdio.h>
#include <stdlib.h>
#include "bag.h"

#define INITIAL_CAPACITY 10

struct bag {
    void **items;
    size_t size;
    size_t capacity;
};

// Create a new bag
bag_t *bag_new() {
    bag_t *newBag = malloc(sizeof(bag_t));
    if (newBag == NULL) {
        fprintf(stderr,"Failed to allocate memory for bag.\n");
        exit(1);
    }

    newBag->items = malloc(INITIAL_CAPACITY * sizeof(void *));
    if (newBag->items == NULL) {
        fprintf(stderr,"Failed to allocate memory for bag items.\n");
        exit(1);
    }

    newBag->size = 0;
    newBag->capacity = INITIAL_CAPACITY;

    return newBag;
}

// Add an item to the bag
bool bag_add(bag_t *bag, void *item) {
    if (bag->size == bag->capacity) {
        bag->capacity *= 2;
        bag->items = realloc(bag->items, bag->capacity * sizeof(void *));
        if (bag->items == NULL) {
            fprintf(stderr,"Failed to reallocate memory for bag items.\n");
            exit(1);
        }
    }

    bag->items[bag->size++] = item;
    return true;
}

// Extract an item from the bag
void *bag_extract(bag_t *bag) {
    if (bag->size == 0) {
        fprintf(stderr,"Error: Attempting to extract from an empty bag.\n");
        exit(1);
    }

    return bag->items[--bag->size];
}

bool bag_is_empty(bag_t *bag) {
    return bag->size == 0;
}

size_t bag_size(bag_t *bag) {
    return bag->size;
}

// Delete the bag
void bag_delete(bag_t *bag, void (*itemdelete)(void *item)) {
    if (itemdelete != NULL) {
        for (size_t i = 0; i < bag->size; i++) {
            itemdelete(bag->items[i]);
        }
    }

    free(bag->items);
    free(bag);
}
