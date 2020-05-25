Will Baginski, CS50 Spring 2020, Lab 6
### Testing Querier
In order to test querier, testing script `testing.sh` was written

It tests case with invalid command line arguments, invalid query syntax, and uses fuzztesting to automate tests with randomly generated queries
The first two scenarios should result in error messages. The fuzztesting generally should not, but it is possible for an invalid character error to be triggered due to the example indexer data containing some punctuation such as '-'
### Test Run
Run with: `make test`
Output of `make test` can be viewed in `testing.out`
