ll Baginski, CS50 2020 Spring Lab 6
## User interface
execute from a command line with usage syntax `./querier pageDirectory indexFilename` where *pageDirectory* is the name of a directory produced by the crawler, and where *indexFilename* is name of a file produced by the indexer.
The user will supply `stdin` to perform queries (if the command line arguments are verified.
## Inputs and outputs
input: `pageDirectory indexFilename`
and then `stdin`
output: results of queries to `stdout`

## Functional decomposition into modules
the query module contains almost all of the code. It verifies arguments, and processes, carries out, and prints out queries.
there are also separate modules for structures used in the code: 
* index
* hashtable
* counters
* counterspair
* scorekeeper
there is also a word module used to alter `char *`
## Psuedo code for logic/algorithm flow
* validate command line arguments
* build an index
* process the query
* validate the syntax of the query
* perform the query and save the results
* sort the results by score
* print the results
* clean up and exit
## Dataflow through modules
* main validates command line arguments
* index constructs the index
* process chops and saves the words from the query
* query validates the syntax of the query and performs it, saving the results
* handle functions handle and/or operations
* a sort function sorts and prints the results
* main cleans up
## Major data structures
* index, a `hashtable_t *` with `char *` words as keys and `counters_t *` as values
* the `counters_t *` have docID keys and word count values
* `char **` words which contains pointers to every word (`char *`) from a query
* `counters_t *` containing docID keys and score pairs. Used for keeping track of the scores of documents
* `counterspair_t *` which holds two points to `counters_t *` structures
* `scorekeeper_t *` which hold two `int`s, a docID and a score
* `scorekeeper_t **` which is an array of pointers to `scorekeeper_t *` structures. Used for sorting the `scorekeeper_t *` structures by score
## Testing plan
Manually test cases with invalid command line arguments and invalid query syntax. Use `fuzzquery.c` and the sample TSE output to automate testing for valid queries.
