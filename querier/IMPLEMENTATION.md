Will Baginski, CS50 Spring 2020, Lab 6
## Implementation
The querier takes two arguments, both of which are mandatoru
* `pageDirectory`
* `indexFilename`

Their requirements are detailed in README.md

### Argument Validation
The following nonzero exit codes may be returned by the querier in the case of unacceptable arguments:
* 1: incorrect number of arguments were supplied
* 2: `pageDirectory` is not a directory produced by the crawler
* 3: `indexFile` is not an already existing, permissable for reading file

### Other Errors
The following nonzero exit codes may be returned by the querier for other reasons:
* 4: dynamic memory allocation for `char **words` failed

### Preparing to Query
After the arguments are validated, an index is loaded from `indexFilename`
Then, the user is prompted for queries. Until the user sends EOF, the user will continually be prompted for queries every time they press enter.
Upon recieving a query, the query is parsed into its individual words and validated by `process`

### Querying
If the query is validated by `process`, then `query` is called
`query` first makes sure that the query has proper syntax. If it does not, the query terminates there
If the query continues, `query` then checks if the query is a simple case -- just one word. If it is, then the query is handled within the function directly
If the query is more complex, its operations are handled by `handle_and` and `handle_or` in the order that they occur
`handle_and` and `handle_or` make use of helper functions `find_intersection` and `find_union` in performing operations
Once the query is complete, the result is passed to `sort_print_counters` in the form of a `counters_t *`

### Handling Results
`sort_print_counters` makes use of a few helper functions to sort the matching documents in decreasing over of their scores, and then print them out
After this, the functions complete and the code returns to `main` for clean up and exiting

### Dynamic Memory Use
Dynamic memory is used in allocating every structure used in the code
Details on the structures are located in DESIGN.md, and further detail in the header files for the structures, found in ../common/ and ../libcs50/

### Error Handling
In the case of improper arguments, querier exits with nonzero status
In the case of a failure to allocate a a structure to hold words, querier exits with nonzer ostatus
Later allocation failures result in the program continuing, but the query that they occur in will be affected
