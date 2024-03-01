#ifndef BAG_H
#define BAG_H

#include <stdbool.h>
#include <stddef.h>

typedef struct bag bag_t;

bag_t *bag_new();

bool bag_add(bag_t *bag, void *item);

void *bag_extract(bag_t *bag);

bool bag_is_empty(bag_t *bag);

size_t bag_size(bag_t *bag);

void bag_delete(bag_t *bag, void (*itemdelete)(void *item));

#endif /* BAG_H */
