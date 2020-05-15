/*
 * Indexer.c
 *
 * Will Baginski, May 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../common/index.h"

// returns 0 if the supplied char *pageDirectory points to a directory produced by the Crawler
// otherwise returns 1
int pageDirectory_validate(char *pageDirectory) {
	// crawlerFile is the path to .crawler inside of pageDirectory
	char *crawler_file = malloc(strlen(pageDirectory) + strlen("/.crawler") + 1);
	// return with an error if malloc failed
	if (crawler_file == NULL) {
		return 1;
	}
	// construct the path to .crawler
	if (pageDirectory[strlen(pageDirectory) - 1] != '/') {
		sprintf(crawler_file, "%s/.crawler", pageDirectory);
	} else {
		sprintf(crawler_file, "%s.crawler", pageDirectory);
	}
	
	// attempt to open .crawler
	// return 0 if we find it, and therefore ID a directory created by the Crawler, return 1 otherwise
	FILE *fp;
	if ((fp = fopen(crawler_file, "r")) == NULL) {
		free(crawler_file);
		return 1;
	} else {
		free(crawler_file);
		fclose(fp);
		return 0;
	}
}

// attempts to read indexFile. If it cannot be read, open it for writing
// if it cannot be written to either, return nonzero. Otherwise return 0
int indexFile_validate(char *indexFile) {
	FILE *fp;
	if ((fp = fopen(indexFile, "r")) == NULL) {
		if ((fp = fopen(indexFile, "w")) == NULL) {
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

/* 
 * processes and validates command-line arguments
 * format: ./indexer pageDirectory indexFilename
 * pageDirectory must be produced by crawler, indexFilename does not need to exist
 */
int main(int argc, char *argv[]) {
	// make sure we have two arguments
	if (argc != 3) {
		fprintf(stderr, "Error: exactly two additional arguments should be supplied by the user\n");
		return 1;
	}

	// make sure pageDirectory exists and contains .crawler
	char *pageDirectory = argv[1];
	if (pageDirectory_validate(pageDirectory) != 0) {
		fprintf(stderr, "Error: pageDirectory should be an existing directory produced by the Crawler\n");
		return 2;
	}
	
	// make sure that indexFile can be opened
	char *indexFile = argv[2];
	if (indexFile_validate(indexFile) != 0) {
		fprintf(stderr, "Error: indexFile should be a valid path to a file\n");
		return 3;
	}

	// create the reverse index
	// keys will be words as char *
	// values will be counters_t *, in which keys are docIDs and values are, well, counts!
	hashtable_t *index = hashtable_new(500);
	
	// build the index
	index_build(pageDirectory, index);

	// save the index
	if (! index_save(indexFile, index)) {
		fprintf(stderr, "Error: index was not properly saved after construction\n");
		return 4;
	}
	
	// clean up index
	index_delete(index);
}
