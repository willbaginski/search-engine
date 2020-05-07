CS50 Lab 4
## Will Baginski, CS50 Spring 2020

### crawler

## Usage
To compile `crawler`, run `make`.
To clean up, run `make clean`.

## Assumptions
The seed url passed to crawler as an argument should be internal to the Dartmouth CS50 server.

The directory passed to crawler as an argument should already exist. It will store .crawler as well as a file for each crawled page. If the user wants to clear that directory, they should do so on their own. Running crawler multiple times on the same directory will likely erase some data from previous executions.

The depth passed to crawler as an argument should be a non-negative integer.
