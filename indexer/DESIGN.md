IGN.md
## User interface ##
execute from a command line with usage syntax
`./indexer pageDirectory indexFilename`
where *pageDirectory* is the pathname of a directory produced by the Crawler, and
where *indexFilename* is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).
## Inputs and Outputs ##
**indexer:**
input: `pageDirectory indexFilename`
output: file with word frequency and IDs
**indextest:**
input: `oldfilename newfilename`
output: hastable data struct
## Functional decomposition into modules ##
the index module contains functions used for building, saving, and otherwise interacting directly with an index
the pagedir module contains a function that builds a `webpage_t` from a crawler output file
the word module contains a function used for manipulating characters within a `char *`
## Pseudo code for logic/algorithmic flow ##
**indexer:** takes a directory of crawler output files, makes an indexer data structure, creates an output file
Process and validate command-line parameters
Initialize data structure index
`index_build(directory, index)`
`index_save(file, index)`
clean up data structures
**indextest:** takes file created by indexer, makes an indexer data structure, checks that the output matches
Process and validate command-line parameters
Initialize data structure index
`index_load(file1, index)`
`index_save(file2, index)`
clean up data structures
## Dataflow through modules ##
main parses parameters, creates a hashtable, and passes the supplied directory and built hashtable to `index_build`
`index_build` uses the files in the directory to populate the hashtable
`index_save` uses the hashtable and the supplied file to write the data from the hashtable to the file
## Major data structures ##
the index itself, which is a hashtable. It's keys are `char *` words and its values are counters
the counters' keys are document IDs, and its values are counts. Both are type `int`
## Testing plan ##
indexer can be simply tested by running the executable, supplied with a crawler output file
if done with a small, known, crawler output file, the indexer output can be manually checked for accuracy
a more rigorous testing procedure can be performed in conjunction with a completed indexertest program
