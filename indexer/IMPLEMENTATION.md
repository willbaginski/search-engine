Will Baginski, CS50 Spring 2020, Lab 5
## Implementation
The indexer takes two arguments, both of which are mandatory:
* `pageDirectory`
* `indexFile`

Their requirements are detailed in README.md

### Argument Validation
The following nonzero exit codes may be returned by the indexer in the case of unacceptable arguments
*1: incorrect number of arguments were supplied
*2: `pageDirectory` is not a directory created by the crawler
*3: `indexFile` could not be opened
*4: the index could not be properly saved to `indexFile` after it was built

### Preparing to Index
After the arguments are validated, the index is created as a 500-slot hashtable.
`index_build(pageDirectory, index)` is then called.

### Index Build
This function is from `index.h`.
The function builds an index from the data contained in `pageDirectory`. It reads every word in the file, and adds every one of 3+ characters to the index as a key.
The associated values are counters, which have docIDs from crawler as keys, and the number of times the word appears in the document as values.
This function modifies the supplied hashtable to build the index and does not return anything.

### Index Save
This function is from `index.h`.
The function takes a constructed index and writes its data to a file. Each line of the file starts with one word from the hashtable. The rest of the line contains associated docID wordcount pairs. Each docID is separated from its associated count with a space, and each pair is separated with a space.
This function returns a boolean. True in the case of success, false otherwise. If the functions returns false, `indexer.c` prints an error and exits with nonzero status.

### Dynamic Memory Use
Dynamic memory is used in allocating the hashtable and counters used in the index's construction. Various `char *` are also dynamically allocated within `index_build` and `index_save` and are all allocated and freed entirely within each of them. `webpage_t` structures are also allocated and freed within the two functions.

### Error Handling
`indexer.c` validates its arguments, and exits nonzero if they are improper. Nonzero exit status also occurs if the index is unable to be properly saved.
