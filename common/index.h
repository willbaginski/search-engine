/*
 * header file for index -- contains function to work with the index data structure
 * 
 *
 * Will Baginski, May 2020
 */

#include <stdbool.h>
#include "../libcs50/hashtable.h"

// given a directory created by Crawler and an empty hashtable, constructs an index
// returns the index
void index_build(char *pageDirectory, hashtable_t *index);

// given an index, saves the index in a specific format in file
// returns true upon success, false otherwise
bool index_save(char *file, hashtable_t *index);

// creates an index from a file created by indexer
// builds the index in the hashtable index supplied as a parameter
void index_load(char *file, hashtable_t *index);

/* deletes an index
 * calls hashtable delete on the index, which takes a helper
 * function as itemdelete which calls counters_delete
 * on each item
 */
void index_delete(hashtable_t *index);
