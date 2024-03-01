#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagedir.h"

/*
int pageID = 0;

int get_pageID() {
    return pageID;
}
void increment_pageID() {
    pageID += 1;
}
webpage_t *pagedir_init(const char *url, int depth) {
    webpage_t *newPage = malloc(sizeof(webpage_t));

    if (newPage == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for webpage.\n");
        free(newPage); 
        exit(1);
    }

    newPage->url = malloc(strlen(url) + 1);
    if (newPage->url == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for URL.\n");
        free(newPage); 
        exit(1);
    }

    strcpy(newPage->url, url);
    newPage->depth = depth;
    increment_pageID();
    newPage->id = get_pageID();

    printf("%d     Added: %s\n", depth, newPage->url);
    
    return newPage;
}

static void insertWebpage(hashtable_t *pagesSeen, bag_t *pagesToCrawl, const char *url, int depth) {
    if (hashtable_insert(pagesSeen, url, NULL)) {
        webpage_t *newPage = malloc(sizeof(webpage_t));
        if (newPage == NULL) {
            fprintf(stderr, "Out of memory.\n");
            exit(1);
        }

        newPage->url = strdup(url);
        newPage->depth = depth;

        if (bag_add(pagesToCrawl, newPage)) {
            newPage->id = bag_size(pagesToCrawl);
            pageID = bag_size(pagesToCrawl);
        } else {
            fprintf(stderr, "Failed to add webpage to the bag.\n");
            deleteWebpage(newPage);
        }
    }
	
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	char filePath[256]; // Adjust the size based on your requirements
    // Create the filename using the document ID
    snprintf(filePath, sizeof(filePath), "%s/%d.txt", pageDirectory, documentID);
	
    FILE *file = fopen(filePath, "w"); //change to a

    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", pageDirectory);
        exit(1);
    }

    // Write URL to file
    fprintf(file, "%s\n", page->url);

    // Write depth to files
    fprintf(file, "%d\n", page->depth);

    // Write HTML content to file
    fprintf(file, "%s", page->html);

    fclose(file);
}

static void deleteWebpage(void *item) {
    if (item != NULL) {
        webpage_t *page = (webpage_t *)item;
        free(page->url);
        free(page);
    }
}
*/
