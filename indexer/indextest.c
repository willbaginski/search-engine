/*
 * indextest -- tests the indexer
 *
 * Will Baginski, May 2020
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"

int main(int argc, char *argv[]) {
	// make sure we have the correct number of arguments
	if (argc != 3) {
		fprintf(stderr, "Error: user should supply exactly two additional arguments\n");
		return 1;
	}
	
	// grab our arguments
	char *oldIndexFilename = argv[1];
	char *newIndexFilename = argv[2];

	// make sure that oldIndexFilename exists
	FILE *fp;
	if ((fp = fopen(oldIndexFilename, "r")) == NULL) {
		fprintf(stderr, "Error: oldIndexFilename should be a file produced by the indexer\n");
		return 2;
	}
	// determine number of lines for use in determining index size
	int lines = lines_in_file(fp);
	fclose(fp);

	// make sure we can write to newIndexFilename
	if ((fp = fopen(newIndexFilename, "w")) == NULL) {
		fprintf(stderr, "Error: newIndexFilename should be a path to writeable file\n");
		return 3;
	}
	fclose(fp);

	// initialize the index
	hashtable_t *index = hashtable_new(1.3 * lines);
	
	// load the index from the old index file
	index_load(oldIndexFilename, index);

	// save the loaded index into a new file
	index_save(newIndexFilename, index);
	
	// clean up
	index_delete(index);
	return 0;
}
