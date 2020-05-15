/*
 * index -- functions for working with an index data structure
 * further detail in index.h
 *
 * Will Baginski, April 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "pagedir.h"
#include "word.h"

// constructs an index from a crawler output directory
void index_build(char *pageDirectory, hashtable_t *index) {
	// format the directory name
	char *dir = malloc(strlen(pageDirectory) + 2);  // make room for '/'
	if (pageDirectory[strlen(pageDirectory) - 1] != '/') {
		sprintf(dir, "%s/", pageDirectory);
	} else {
		sprintf(dir, "%s", pageDirectory);
	}
	
	// initialize first pathname
	int docID = 1;
	char *doc = malloc(strlen(dir) + 11);  // enough space for directories containing up to ten billion pages
	sprintf(doc, "%s%d", dir, docID);
	FILE *fp;

	// go through each file in the directory
	while ((fp = fopen(doc, "r")) != NULL) {
		// loads a webpage_t from the contents of the file
		webpage_t *page = page_load(fp);

		// reads every word in the file
		int pos = 0;
		char *word;
		while ((word = webpage_getNextWord(page, &pos)) != NULL) {
			// make sure word is 3+ characters
			if (strlen(word) > 2) {
				// convert the word to lowercase
				NormalizeWord(word);
				// if the word is not in the table yet, insert word, new counters_t into the hashtable
				if (hashtable_find(index, word) == NULL) {
					hashtable_insert(index, word, counters_new());
				}
				// select the right ctrs to update
				counters_t *ctrs = hashtable_find(index, word);
				// increment the counters for the specific word docID pair
				counters_add(ctrs, docID);
			}
			// clean up
			free(word);
		}
		// move on to next document
		webpage_delete(page);
		fclose(fp);
		docID++;
		sprintf(doc, "%s%d", dir, docID);
	}
	// clean up
	free(dir);
	free(doc);
}

// declaring two helper functions. defintions and details below
void hashtable_helper(void *arg, const char *key, void *item);
void counters_helper(void *arg, const int key, int count);

// saves an index to a file
bool index_save(char *file, hashtable_t *index) {
	FILE *fp;
	if ((fp = fopen(file, "w")) == NULL) {
		return false;
	}
	// iterate over every ctrs, and then each docID count pair within them
	hashtable_iterate(index, fp, hashtable_helper);
	
	// clean up
	fclose(fp);
	return true;
}

// helps index_save write the index to a file
// internal function not meant for outside users of the module
void hashtable_helper(void *arg, const char *key, void *item) {
	FILE *fp = arg;
	// key is the word, which does not need to be cast
	counters_t *ctrs = item;
		
	fprintf(fp, "%s", key);  // prints the word at the start of a line
	counters_iterate(ctrs, fp, counters_helper);  // prints each docID count pair
	fprintf(fp, "\n");  // moves on to the next line
}

// helps hashtable_helper print the contents of each counters within index to a file
// internal function not meant for outside users of the module
void counters_helper(void *arg, const int key, int count) {
	FILE *fp = arg;
	fprintf(fp, " %d %d", key, count);  // prints a docID count pair
}

// loads an index from a file created by the indexer
void index_load(char *file, hashtable_t *index) {
	FILE *fp;
	if ((fp = fopen(file, "r")) != NULL) {
		// read the word on the line
		char *word;
		while ((word = freadwordp(fp)) != NULL) {
			// strip each docID count pair from the remainder of the line
			int docID;
			int count;
			// take each docID count pair and put them into the right counters
			while ((fscanf(fp, "%d %d ", &docID, &count)) == 2) {
				if (hashtable_find(index, word) == NULL) {
					hashtable_insert(index, word, counters_new());
				}
				counters_t *ctrs = hashtable_find(index, word);
				counters_set(ctrs, docID, count);
			}
			// clean up
			free(word);
		}
		// clean up
		fclose(fp);
	}
}

// see definition below
void index_delete_helper(void *ctrs);

// deletes an index
void index_delete(hashtable_t *index) {
	hashtable_delete(index, index_delete_helper);
}

// helps index_delete delete the counters
void index_delete_helper(void *ctrs) {
	counters_delete(ctrs);
}
