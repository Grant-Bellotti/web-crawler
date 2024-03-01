#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct hashtable hashtable_t;

typedef size_t (*hash_function_t)(const char *key);

typedef bool (*key_compare_function_t)(const char *key1, const char *key2);

typedef void (*free_function_t)(void *data);

typedef void *(*clone_function_t)(const void *data);

hashtable_t *hashtable_new(size_t capacity);

bool hashtable_insert(hashtable_t *hashtable, const char *key, void *data);

bool hashtable_find(hashtable_t *hashtable, const char *key);

void hashtable_remove(hashtable_t *hashtable, const char *key, free_function_t free_func);

void hashtable_delete(hashtable_t *hashtable, free_function_t free_func);

#endif  // HASHTABLE_H
