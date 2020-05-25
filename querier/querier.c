/*
 * querier.c -- accepts queries from the user and returns the result
 *
 * Will Baginski, May 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "../libcs50/file.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../common/counterspair.h"
#include "../common/scorekeeper.h"

// returns true if the supplied char *pageDirectory is a directory produced by the crawler
bool pageDirectory_validate(char *pageDirectory);

// returns true if the supplied char *indexFilename is a path to a valid file
bool indexFilename_validate(char *indexFilename);

// puts each word from char *line into a char **words
// also keeps track of the number of items in the array with word_num
// returns true upon success, false in the case of any errors
bool process(char *line, char **words, int *word_num);

// given an array of char *, performs a search query taking the words in the order in
// which they appear in the array. makes use of the supplied directory and file
// returns false if the query in unsuccessful
bool query(char *pageDirectory, hashtable_t *index, char *words[], int word_num);

// adds a word, wordstart, to an array of char *, words
bool addword(char **words, char *wordstart, int *word_num);

// handles query OR operation
void handle_or(hashtable_t *index, char **words, counters_t *res_and, counters_t *res_or, int *j, int word_num);

// handles query AND operation
// saves intersection of res_and and second word in the operation into res_and
void handle_and(hashtable_t *index, char **words, counters_t *res_and, int *j, bool implicit_and);

// helper function for handle_or
// used as the itemfunc in a call of counters_iterate
// saves the union of two counters into the counters supplied as void *arg
void find_union(void *arg, const int key, const int count);

// helper function for handle_and
// used as the itemfunc in a call of counters_iterate
// saves the intersection of two counters into the counters being iterated over
// by the call of counters_iterate in handle_and
// takes a counterspair as arg
// should ONLY be called by handle_and
void find_intersection(void *arg, const int key, const int count);

// take a counters and sorts its content into an array containing scorekeeper structures
// (sorts by score, in decreasing order)
// a scorekeeper contains two ints: a docID and a score
// then prints the items + information about them to give the result of the query
void sort_print_counters(char *pageDirectory, counters_t *ctrs);

// counts the number of items in a counters
// takes a pointer to an int as arg
// (helper function for sort_counters)
void count_helper(void *arg, const int key, const int count);

// counters the number of items in a counters with score > 0
// takes a pointer to an int as arg
// (helper function for sort_counters)
void match_helper(void *arg, const int key, const int count);

// takes items from a counters, make scorekeepers from their values, and inserts
// them into an array in order by score (largest score first)
// (helper function for sort_counters)
void sort_helper(void *arg, const int key, const int count);

// validates command-line arguments
// calls functions to handle queries and output of results
int main(int argc, char *argv[]) {
	// make sure we have the right number of arguments
	if (argc != 3) {
		fprintf(stderr, "Error: the user should supply exactly two additional arguments.\n");
		return 1;
	}

	// make sure that pageDirectory was produced by crawler
	char *pageDirectory = argv[1];
	if (! pageDirectory_validate(pageDirectory)) {
		fprintf(stderr, "Error: pageDirectory should be an existing directory produced by the crawler.\n");
		return 2;
	}

	// make sure that indexFilename is a readable file
	char *indexFilename = argv[2];
	if (! indexFilename_validate(indexFilename)) {
		fprintf(stderr, "Error: indexFile should be a valid path to a file.\n");
		return 3;
	}
	
	// build the index (do it here so that it will never have to be rebuilt)
	FILE *fp;
	fp = fopen(indexFilename, "r");
	int lines = lines_in_file(fp);
	fclose(fp);
	hashtable_t *index = hashtable_new(1.3 * lines);
	index_load(indexFilename, index);

	// ask the user for queries
	// if final line doesn't have '\n' before EOF, then it won't be processed
	printf("Query? ");
	char *line;
	while ((line = freadlinep(stdin)) != NULL) {
		// print the line again
		printf("Query: %s\n", line);
		// process line into words
		int word_num;
		char **words = malloc((1 + strlen(line) / 2) * sizeof(char*));
		if (words == NULL) {
			fprintf(stderr, "Error: array of words was not properly allocated.\n");
			return 4;
		}
		if (process(line, words, &word_num)) {
			// perform the query
			if (word_num > 0) {
				query(pageDirectory, index, words, word_num);
			} else {
				printf("Empty query.\n");
			}
		}
		// cleanup
		for (int j = 0; j < word_num; j++) {
			free(words[j]);
		}
		free(words);
		free(line);
		printf("\nQuery? ");  // ask for next query
	}
	// clean up and exit
	printf("\n");  // neaten up
	index_delete(index);
	return 0;
}

// described in declaration above
bool pageDirectory_validate(char *pageDirectory) {
	// crawlerFile is the path to .crawler inside of pageDirectory
	char *crawler_file = malloc(strlen(pageDirectory) + strlen("/.crawler") + 1);
	// return false if malloc failed
	if (crawler_file == NULL) {
		return false;
	}
	// construct the path to .crawler
	if (pageDirectory[strlen(pageDirectory) - 1] != '/') {
		sprintf(crawler_file, "%s/.crawler", pageDirectory);
	} else {
		sprintf(crawler_file, "%s.crawler", pageDirectory);
	}

	// attempt to open .crawler
	// return true if we find it, false otherwise
	FILE *fp;
	if ((fp = fopen(crawler_file, "r")) == NULL) {
		free(crawler_file);
		return false;
	}
	free(crawler_file);
	fclose(fp);
	return true;
}

// described in declaration above
bool indexFilename_validate(char *indexFilename) {
	FILE *fp;
	if ((fp = fopen(indexFilename, "r")) == NULL) {
		return false;
	}
	fclose(fp);
	return true;
}

// described in declaration above
bool process(char *line, char **words, int *word_num) {
	// initialize variables
	*word_num = 0;
	char *wordstart = line;
	bool reset = false;
	
	// read every char in the string
	for (char *cp = line; *cp != '\0'; cp++) {

		// reset wordstart, if necessary
		if (reset) {
			wordstart = cp;
			reset = false;
		}
		// if we encounter a space, replace it with an EOS
		// then add the word ending there to the array of words
		if (isspace(*cp)) {
			*cp = '\0';  // mark the end of the word
			// add the finished word and prepare for the next word
			if (strlen(wordstart) > 1 || isalpha(*wordstart)) {
				if (! addword(words, wordstart, word_num)) {
					fprintf(stderr, "Error: words from query were not properly added.\n");
					return false;
				}
			}
			// slate wordstart to be updated at the start of the next iteration
			reset = true;
		
		// we don't have a space, so no special action is required
		// just make sure the character is legal (alphabetical)
		} else {
			// if the word contains a nonalphabetical character, the query is illegal
			// print an error and exit
			if (! isalpha(*cp)) {
				fprintf(stderr, "Error: query contains an illegal character.\n");
				return false;
			}
		}
	}
	// add the last word to words
	if (strlen(wordstart) > 1 || isalpha(*wordstart)) {
		if (! addword(words, wordstart, word_num)) {
			fprintf(stderr, "Error: words from query were not properly added.\n");
			return false;
		}
	}
	// exit
	return true;
}

// described in declaration above
bool addword(char **words, char *wordstart, int *word_num) {
	NormalizeWord(wordstart);
	words[*word_num] = malloc(strlen(wordstart) + 1);
	
	if (words[*word_num] == NULL) {
		fprintf(stderr, "Error: memory allocation for a word in the query failed.\n");
		return false;
	}
	strcpy(words[*word_num], wordstart);
	(*word_num)++;
	return true;
}

// described in declaration above
bool query(char *pageDirectory, hashtable_t *index, char *words[], int word_num) {
	// make sure the query does not begin with an operator or have consecutive operators
	bool prev_op = false;
	for (int i = 0; i < word_num; i++) {
		if (strcmp(words[i], "or") == 0 || strcmp(words[i], "and") == 0) {
			if (i == 0 || i == word_num - 1) {
				fprintf(stderr, "Error: invalid query syntax (query begins or ends with an operator).\n");
				return false;
			} else if (prev_op) {
				fprintf(stderr, "Error: invalid query syntax (consecutive operators).\n");
				return false;
			} else {
				prev_op = true;
			}
		} else {
			prev_op = false;
		}
	}

	// check if we only have one word -- if so, we don't have to worry about special operations
	if (word_num == 1) {
		counters_t *docs = hashtable_find(index, words[0]);
		if (docs == NULL) {
			printf("No documents match the query.\n");
			return true;
		} else {  // we do have matches
			sort_print_counters(pageDirectory, docs);
			return true;
		}
	}
	
	// prepare variables used in the query
	// booleans used in determining which operation to perform
	bool or = false;
	bool implicit_and = false;
	// two counters for unions and intersections
	counters_t *res_or = NULL;
	counters_t *res_and = counters_new();  // first word always goes to res_and
	counters_iterate(hashtable_find(index, words[0]), res_and, find_union);
	// int to keep track of which word is being references
	int j = 0;
	// begin query
	while (j < word_num - 1) {
		// determine type of operation
		if (strcmp(words[j + 1], "or") == 0) {
			or = true;
		} else if (strcmp(words[j + 1], "and") != 0) {
			implicit_and = true;
		}

		// handle or operation
		if (or) {
			handle_or(index, words, res_and, res_or, &j, word_num);
			or = false;
		// if it's not an or operation then it's an and operation
		// handle and operation (implicit or regular)
		} else {
			handle_and(index, words, res_and, &j, implicit_and);
			implicit_and = false;
		}
	}
	// sort the resulting counters and print it
	sort_print_counters(pageDirectory, res_and);
	counters_delete(res_and);
	return true;
}

// described in declaration above
void handle_or(hashtable_t *index, char **words, counters_t *res_and, counters_t *res_or, int *j, int word_num) {
	// determine the state after the current operation: or, and, or end of query
	// note that words[j] is currently the word before "or"
	
	// upon encountering an or, the content of res_and is switched to res_or for "safekeeping"
	res_or = res_and;
	// first case: when there is another or operation or end of query following the current or
	if (*j + 3 == word_num || strcmp(words[*j + 3], "or") == 0) {
		// straightforward: put the union of res_or and the next word into res_or
		counters_t *r_docs = counters_new();
		counters_iterate(hashtable_find(index, words[*j + 2]), r_docs, find_union);
		if (r_docs != NULL) {
			counters_iterate(r_docs, res_or, find_union);
		}
		counters_delete(r_docs);
		// increment j to note that the operation is completed
		*j += 2;
		// tranfer res_or to res_and (we assume that the result of an operation always ends up in res_and)
		res_and = res_or;

	// second case: and operation follows the current or
	} else {
		// move to the first word of the and operation
		*j += 2;
		// save the current word in res_and (previous content of res_and is safely stored in res_or)
		res_and = counters_new();
		counters_iterate(hashtable_find(index, words[*j]), res_and, find_union);

		// perform and operations until an or operation of end of query is reached
		while (*j < word_num - 1 && strcmp(words[*j + 1], "or") != 0) {
			handle_and(index, words, res_and, j, strcmp(words[*j + 1], "and") != 0);
		}
		// return to the or operation
		// save union of res_and and res_or in res_or
		counters_iterate(res_and, res_or, find_union);
		counters_delete(res_and);
	}
}

// described in declaration above
void handle_and(hashtable_t *index, char **words, counters_t *res_and, int *j, bool implicit_and) {
	// grab the counters from the index for the left word
	if (res_and != NULL) {
		// index to the right word (either one or two steps)
		(*j)++;
		if (! implicit_and) {
			(*j)++;
		}
		// grab the counters from the index for the right word
		counters_t* r_docs = counters_new();
		counters_iterate(hashtable_find(index, words[*j]), r_docs, find_union);
		if (r_docs != NULL) {
			// find the intersection between left and right
			// create a structure holding both pairs (defined in ../common/counterspair.h)
			counterspair_t *docs = counterspair_new(res_and, r_docs);
			// find the intersection using the counterspair and a helper function
			counters_iterate(counterspair_getOne(docs), docs, find_intersection);
			// clean up and exit
			free(docs);
			counters_delete(r_docs);
		}
	}
}

// described in declaration above
void find_union(void *arg, const int key, const int count) {
	counters_t *res_or = arg;
	// for key in res_or, update the value to be the count in res_and + the count already in res_or
	int score = counters_get(res_or, key) + count;
	counters_set(res_or, key, score);
}

// described in declaration above
void find_intersection(void *arg, const int key, const int count) {
	counterspair_t *docs = arg;
	counters_t *res_and = counterspair_getOne(docs);
	counters_t *r_docs = counterspair_getTwo(docs);

	// check if current key from res_and (what we are iterating over) is also in r_docs
	// if it is, adjust score in res_and
	// if it is not, set to zero in res_and
	int r_docs_count = counters_get(r_docs, key);
	if (r_docs_count == 0) {
		// the doc associated with this key is not a match
		// set its score to zero
		counters_set(res_and, key, 0);
	} else {
		// the doc associated with this key is a match
		// if the count in r_docs is less than the count in l_docs, update the count
		// in l_docs to the count of r_docs (score is minimum b/t the two docs)
		if (r_docs_count < count) {
			counters_set(res_and, key, r_docs_count);
		}
	}
}

// described in declaration above
void sort_print_counters(char *pageDirectory, counters_t *ctrs) {
	// determine the number of entries in the counters
	int size = 0;
	counters_iterate(ctrs, &size, count_helper);
	// allocate an array of the correct size
	scorekeeper_t **array = calloc(size, sizeof(scorekeeper_t *));
	// count number of documents with score>0
	int doc_num = 0;
	counters_iterate(ctrs, &doc_num, match_helper);
	// sort the array
	counters_iterate(ctrs, array, sort_helper);
	// print the items in the array
	printf("--------------------------------------------------------------\n");
	if (doc_num > 0) {
		printf("Matches %d documents (ranked):\n", doc_num);
	} else {
		printf("No documents match.\n");
	}
	for (int i = 0; i < size; i++) {
		// grab docID, score
		int docID = scorekeeper_getDocID(array[i]);
		int score = scorekeeper_getScore(array[i]);
		if (score > 0) {
			// print the result
			printf("Score %d ", score);  // print score
			printf("Doc %d: ", docID);  // print docID
			
			// grab url from first line of the file in pageDirectory (we assume less than one billion documents)
			// construct path to document info
			char *path;
			if (pageDirectory[strlen(pageDirectory) - 1] != '/') {
				path = malloc(strlen(pageDirectory) + 11);
				sprintf(path, "%s/%d", pageDirectory, docID);
			} else {
				path = malloc(strlen(pageDirectory) + 10);
				sprintf(path, "%s%d", pageDirectory, docID);
			}
			FILE *fp;
			fp = fopen(path, "r");
			char *url = freadlinep(fp);
			printf("%s\n", url);  // print url
			fclose(fp);
			// clean up after each iteration
			free(url);
			free(path);
		}
		free(array[i]);
	}
	// clean up
	free(array);
}

// described in declaration above
void count_helper(void *arg, const int key, const int count) {
	int *size = arg;
	(*size)++;
}

// described in declaration above
void match_helper(void *arg, const int key, const int count) {
	int *doc_num = arg;
	if (count > 0) {
		(*doc_num)++;
	}
}

// described in declaration above
void sort_helper(void *arg, const int key, const int count) {
	// grab the array that we will insert into and sort
	scorekeeper_t **array = arg;
	// create a scorekeeper from the key/count
	scorekeeper_t *scorekeeper = scorekeeper_new(key, count);
	// insert into the correct slot
	// find the right index for the new item
	int i = 0;
	while (scorekeeper_getScore(scorekeeper) < scorekeeper_getScore(array[i])) {
		i++;
	}
	// find the index of the last non-NULL entry
	int j = i;
	while (array[j] != NULL) {
		j++;
	}
	// shift all the scorekeepers from indices i:j one spot right (make them i+1:j+1)
	for (int k = j - 1; k >= i; k--) {
		array[k + 1] = array[k];
	}

	// insert scorekeeper
	array[i] = scorekeeper;
}
