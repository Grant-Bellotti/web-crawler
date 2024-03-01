#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdlib.h>
#include <stddef.h>

#include "hashtable.h"
#include "bag.h"
#include "url.h"
#include "curl.h"

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
	int id;
} webpage_t;

static void insertWebpage(hashtable_t *pagesSeen, bag_t *pagesToCrawl, const char *url, int depth);

static void deleteWebpage(void *item);

static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth);

static void crawl(bag_t *pagesToCrawl, hashtable_t *pagesSeen, const char *pageDirectory, const int maxDepth);

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);

static void insertWebpage(hashtable_t *pagesSeen, bag_t *pagesToCrawl, const char *url, int depth);

void writePageToFile(const char *pageDirectory, int id, webpage_t *page, const char *html);

#endif
