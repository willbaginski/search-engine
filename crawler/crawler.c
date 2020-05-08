/*
 * crawler.c
 *
 * crawls through webpages, given a starting point, a depth, and page directory
 *
 * Will Baginski, April 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"

// makes sure that the supplied directory is writeable
// adds '/' to the end of it if it is not present
// creates ".crawler" inside the directory if it is not present
int page_validate(char *directory) {
	char filename[100];
	// account for both formats of directory argument
	if (directory[strlen(directory) - 1] != '/') {
		sprintf(filename, "%s/.crawler", directory);
	} else {
		sprintf(filename, "%s.crawler", directory);
	}
	// check is filename is readable
	FILE *fp;
	if ((fp = fopen(filename, "r")) == NULL) { // file doesn't exist yet, we should make it
		fp = fopen(filename, "w");
		if (fp == NULL) {
			fprintf(stderr, "Error: directory was not writeable or does not exist\n");
			return 1;
		}
	}
	fclose(fp);
	return 0;
}


// adds the html to a webpage
// returns true upon success, false otherwise
bool page_fetch(webpage_t *page) {
	return webpage_fetch(page);
}


// takes URLS from a page and inserts the internal ones into a hashtable
// if that insert succeeds, a webpage created with the url is created and inserted into the bag
void page_scan(webpage_t *page, bag_t *bag, hashtable_t *ht) {
	int pos = 0;
	char *url;
	while ((url = webpage_getNextURL(page, &pos)) != NULL) {  // only go further is there is another url
		char *urlcpy = malloc((strlen(url) + 1) * sizeof(char));
		strcpy(urlcpy, url);
		if (IsInternalURL(urlcpy)) {  // only add the url if it is internal
			if (hashtable_insert(ht, urlcpy, "")) {  // only add the page to the bag if the url hasn't been seen before
				#ifdef MYTEST
					printf("added to bag: %s\n", urlcpy);
				#endif
				bag_insert(bag, webpage_new(urlcpy, webpage_getDepth(page) + 1, NULL));
			} else {
				free(urlcpy);
			}
		} else {
			free(urlcpy);
		}
		// free the url
		free(url);
	}
}

// starting from a seedURL, crawler records data from internal pages linked to on
// the page corresponding to seedURL in a supplied directory. Then crawls those pages,
// and continues to a specified depth
int crawler(char *seedURL, char *directory, int maxDepth) {
	webpage_t *seed = webpage_new(seedURL, 0, NULL);
	if (seed == NULL) {
		free(seedURL);
		fprintf(stderr, "Error: webpage could not be created from seedURL\n");
		return 5;
	}

	// construct the bag and hashtable
	bag_t *bag = bag_new();
	hashtable_t *ht = hashtable_new(200);  // number of slots hardcoded

	// prepare the structures for crawling
	bag_insert(bag, seed);
	hashtable_insert(ht, seedURL, "");

	// crawl
	int ID = 0;
	int exitcode = 0;
	webpage_t *page;
	while ((page = bag_extract(bag)) != NULL) {
		#ifdef MYTEST
			printf("current page: %s\n", webpage_getURL(page));
		#endif
		// attempt to fetch html
		if (page_fetch(page)) {
			// save the page
			page_saver(page, directory, ID);
			ID++;
			
			// get more pages if we are not at maxDepth
			if (webpage_getDepth(page) < maxDepth) {
				page_scan(page, bag, ht);
			}
		} else {
			fprintf(stderr, "Error: html for %s was unable to be fetched\n", webpage_getURL(page));
			exitcode = 6;
		}
		// delete the page we extracted
		webpage_delete(page);
	}
	// clean up and exit
	bag_delete(bag, webpage_delete);
	hashtable_delete(ht, NULL);
	return exitcode;
}


// validates command-line arguments
int main(int argc, char *argv[]) {
	// first make sure we have three arguments
	if (argc != 4) {
		fprintf(stderr, "Error: crawler should be supplied with three arguments\n");
		return 1;
	}

	// make sure that maxDepth is a non-negative integer
	int maxDepth;
	if (sscanf(argv[3], "%d", &maxDepth) != 1 || maxDepth < 0) {
		fprintf(stderr, "Error: the depth argument should be a non negative integer\n");
		return 2;
	}

	/* make sure that the directory supplied exists and is writeable
	 * add '/' to the end if necessary
	 * create .crawler if it does not already exist
	 */
	char *directory = argv[2];
	if (page_validate(directory) != 0) {
		return 3;
	}

	char *seedURL = malloc(strlen(argv[1]) + 1);
	strcpy(seedURL, argv[1]);
	if (! IsInternalURL(seedURL)) {
		fprintf(stderr, "Error: url supplied as argument should be internal to the CS50 server\n");
		return 4;
	}

	return crawler(seedURL, directory, maxDepth);
}
