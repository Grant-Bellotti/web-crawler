CC = gcc
SOURCE = hashtable.c bag.c curl.c url.c pagedir.c crawler.c 
OBJ = hashtable.o bag.o curl.o url.o pagedir.o crawler.o 
HEADER = crawler.h

.PHONY: all clean

all: crawler

crawler: $(OBJ)
	$(CC) $^ -o $@ -Wall -Wextra -Wpedantic -lcurl

%.o: %.c $(HEADER)
	$(CC) -c $< -o $@

clean:
	rm -rf crawler
	rm -rf $(OBJ)
