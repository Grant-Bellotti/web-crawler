#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

typedef struct entry {
    char *key;
    void *data;
    struct entry *next;
} entry_t;

struct hashtable {
    size_t size;
    entry_t **table;
};

// Hash function
static size_t hash(const char *key, size_t size) {
    size_t hashValue = 0;
    for (; *key != '\0'; key++) {
        hashValue = *key + 31 * hashValue;
    }
    return hashValue % size;
}

// Create a new hashtable
hashtable_t *hashtable_new(size_t size) {
    hashtable_t *newTable = malloc(sizeof(hashtable_t));
    if (newTable == NULL) {
        return NULL; 
    }

    newTable->size = size;
    newTable->table = calloc(size, sizeof(entry_t *));
    if (newTable->table == NULL) {
        free(newTable);
        return NULL; 
    }

    return newTable;
}

// Create a new hashtable entry
static entry_t *create_entry(const char *key, void *data) {
    entry_t *newEntry = malloc(sizeof(entry_t));
    if (newEntry == NULL) {
        return NULL;
    }

    newEntry->key = strdup(key);
    if (newEntry->key == NULL) {
        free(newEntry);
        return NULL; 
    }

    newEntry->data = data;
    newEntry->next = NULL;

    return newEntry;
}

// Insert into hashtable
bool hashtable_insert(hashtable_t *hashtable, const char *key, void *data) {
    if (hashtable == NULL || key == NULL) {
        return false;
    }

    size_t index = hash(key, hashtable->size);

    entry_t *current = hashtable->table[index];
    while (current != NULL) { // check if the key already exists
        if (strcmp(current->key, key) == 0) {
            current->data = data;
            return true;
        }
        current = current->next;
    }

    entry_t *newEntry = create_entry(key, data);
    if (newEntry == NULL) {
        return false;
    }

    newEntry->next = hashtable->table[index];
    hashtable->table[index] = newEntry;

    return true;
}

// Find a key in the hashtable
bool hashtable_find(hashtable_t *hashtable, const char *key) {
    if (hashtable == NULL || key == NULL) {
        return false;
    }

    size_t index = hash(key, hashtable->size);
    entry_t *current = hashtable->table[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

// Delete a hashtable
void hashtable_delete(hashtable_t *hashtable, void (*itemdelete)(void *)) {
    if (hashtable == NULL) {
        return;
    }

    for (size_t i = 0; i < hashtable->size; i++) {
        entry_t *current = hashtable->table[i];

        while (current != NULL) {
            entry_t *temp = current;
            current = current->next;

            if (itemdelete != NULL) {
                itemdelete(temp->data);
            }

            free(temp->key);
            free(temp);
        }
    }

    free(hashtable->table);
    free(hashtable);
}
