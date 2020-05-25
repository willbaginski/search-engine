Will Baginski, CS50 Spring 2020, Lab 6
## Querier
### Usage
To compile `querier.c` and `fuzzquery.c`: `make`
To clean: `make clean`
To run test script: `make test`

### Assumptions
Arguments supplied by the user are validated
It is assumed that the user does not wish to search for:
* words with fewer than three characters
* any non-alphabetical characters such as numbers or punctuation
* the word "and"
* phrase or exact terms where words separated by spaces occur in a specific order (such as "Dartmouth College")
* capital letters (all words are normalized to being entirey lowercase. Thus, there is no distinction between "will" and "Will")
Functionality for the listed types of searches is not supported
