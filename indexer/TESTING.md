### Will Baginski, CS50 Spring 2020 Lab 5
### Testing Indexer
In order to test Indexer, testing script `testing.sh` and a program `indextester.c` were written.

`indextester.c` takes an output file created by `indexer.c`, uses it to rebuild an index, and then creates a new output file from that rebuilt index.
If the Indexer is working properly, the new output file should contain exactly the same information as the original output file from the run of `indexer.c`.

Bash script in `testing.sh` is used to make the process more clear, and to test error catching in both `indexer.c` and `indextest.c`. First, the script runs each program with a variety of errors in the command-line arguments. These runs are meant to check if each program is properly validating its arguments, and they intentionally result in error messages and nonzero exit status.

The script then goes on to run `indexer.c` on an example crawler output directory and produce an output file `testIndex`. Afterward it runs `indextest.c`, supplied with `testIndex` to produce `testCopy`. The two files are then reorganized by `indexsort.awk` so that their data appears in a consistent, predictable format (saved as `testIndex.sorted` and `testCopy.sorted`. Their contents are then compared with `diff testIndex.sorted testCopy.sorted`. 

### Test Run
Run with: `make test`.
Output of `make test` can be viewed in `testing.out`.
