//gcc -o crawler hashtable.c bag.c curl.c url.c crawler.c -lcurl
//./crawler https://www.ucsc.edu/ data 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crawler.h"

int pageID = 0;

static void printUsage() {
    fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
    exit(1);
}

void savePage(const char *pageDirectory, int id, webpage_t *page) {
////////////////////////////////// makes the ‘directory produced by the Crawler’
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%s.crawler", pageDirectory, pageDirectory);
    FILE *ident = fopen(filePath, "w");
    fclose(ident);
/////////////////////////////////
    snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, id);
    FILE *file = fopen(filePath, "w");

    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", pageDirectory);
        exit(1);
    }

    fprintf(file, "%s\n", page->url);
    fprintf(file, "%d\n", page->depth);
    fprintf(file, "%s", page->html);

    fclose(file);
}
/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        printUsage();
    }

    *seedURL = argv[1];
    *pageDirectory = argv[2];

    char *endptr;
    long parsedDepth = strtol(argv[3], &endptr, 10);

    if (*endptr != '\0' || parsedDepth < 0 || parsedDepth > 10) {
        fprintf(stderr, "Error: Invalid maxDepth. Must be an integer in the range [0, 10].\n");
        printUsage();
    }

    *maxDepth = (int)parsedDepth;
}

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(bag_t *pagesToCrawl, hashtable_t *pagesSeen, const char *pageDirectory, const int maxDepth) {
    while (!bag_is_empty(pagesToCrawl)) {
        webpage_t *currentPage = bag_extract(pagesToCrawl);

        char *html = download(currentPage->url, &currentPage->length);
        printf("%d    Fetched %s\n", currentPage->depth, currentPage->url);

        if (html != NULL) {
            currentPage->html = html;
            if (currentPage->depth < maxDepth) { // if the web page is not at maxDepth, pageScan   
                printf("%d   Scanning: %s\n", currentPage->depth, currentPage->url);
                pageScan(currentPage, pagesToCrawl, pagesSeen);
            }

            savePage(pageDirectory, currentPage->id, currentPage);

            deleteWebpage(currentPage);
        }
    }
}

webpage_t *createWebpage(const char *url, int depth) {
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
    newPage->id = ++pageID;

    printf("%d     Added: %s\n", depth, newPage->url);
    
    return newPage;
}

bool checkCharacter(const char *str) {
    return (strchr(str, '#') != NULL) ? true : false;
}

/**
 * Scans a webpage for URLs.
 */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) {
    if (page->html == NULL) {
        fprintf(stderr, "Error: HTML content is NULL for the current webpage.\n");
        return;
    }

    char *htmlCopy = strdup(page->html);

    if (htmlCopy == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for HTML copy.\n");
        exit(1);
    }

    const char *searchString = "<a href=";
    const char *ptr = htmlCopy;
    while ((ptr = strstr(ptr, searchString)) != NULL) {
        ptr += strlen(searchString);
        const char *startQuote = strchr(ptr, '"');
        const char *endQuote = strchr(startQuote + 1, '"');
        if (startQuote != NULL && endQuote != NULL) {
            size_t linkLength = endQuote - startQuote - 1; // Extract the link between the quotes
            char link[linkLength + 1];
            strncpy(link, startQuote + 1, linkLength);
            link[linkLength] = '\0';

            char *normalizedURL = normalizeURL(page->url, link);
            printf("%d     Found: %s\n", page->depth, normalizedURL);

            if (!isInternalURL(page->url, normalizedURL)) {
                printf("%d   IgnExtrn: %s\n", page->depth, page->url);
            }
            if (!checkCharacter(normalizedURL) && isInternalURL(page->url, normalizedURL) && !hashtable_find(pagesSeen, normalizedURL)) {
                if (hashtable_insert(pagesSeen, normalizedURL, NULL)) {
                    webpage_t *newPage = createWebpage(normalizedURL, page->depth + 1); // Increment depth

                    if (bag_add(pagesToCrawl, newPage)) {
                        continue;
                    } else {
                        fprintf(stderr, "Failed to add webpage to the bag.\n");
                        free(htmlCopy);
                        exit(1);
                    }
                    
                } else {
                    fprintf(stderr, "Failed to insert webpage into hashtable.\n");
                    free(htmlCopy);
                    exit(1);
                }
            }
        } else {
            ptr += 1; // move to next link
        }
    }

    free(htmlCopy);
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

static void deleteWebpage(void *item) {
    if (item != NULL) {
        webpage_t *page = (webpage_t *)item;
        free(page->url);
        free(page);
    }
}

int main(const int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        printUsage();
    }

    char *seedURL;
    char *pageDirectory;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    bag_t *pagesToCrawl = bag_new();
    hashtable_t *pagesSeen = hashtable_new(100);

    insertWebpage(pagesSeen, pagesToCrawl, seedURL, 0);

    crawl(pagesToCrawl, pagesSeen, pageDirectory, maxDepth);

    bag_delete(pagesToCrawl, deleteWebpage);
    hashtable_delete(pagesSeen, NULL);

    return 0;
}


