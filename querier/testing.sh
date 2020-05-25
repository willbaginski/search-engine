#!/bin/bash

# compile
make clean
make

# test some cases where commandline arguments are not valid
# wrong number of arguments:
./querier 
./querier one
./querier one two three

# directory is not a directory created by crawler
./querier ../querier ../tse-output/letter-index-2

# indexfile is not an existing file
./querier ../tse-output/letters-depth-2 ../tse-output/letters-depth-2/999

# now some cases with invalid syntax
# note that because stdin is being automatically placed in instead of types by the user, the output format is a little funky
# when the query is restated, it looks like "Query? Query: ..."
# when the user types in stdin, "Query: ..." appears on a new line
echo 'and' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2

echo 'algorithm or 10' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2

echo 'playground or or tse' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2

echo 'page and home and' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2

echo 'and or and' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2

echo 'or and' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2

echo 'page home or' | ./querier ../tse-output/letters-depth-2 ../tse-output/letters-index-2
# now use fuzztesting to generate random cases
# first some simple ones (easier to eyeball if the result is correct)
./fuzzquery ../tse-output/letters-index-4 5 504 | ./querier ../tse-output/letters-depth-4 ../tse-output/letters-index-4

# now some complex ones
./fuzzquery ../tse-output/wikipedia-index-1 10 411 | ./querier ../tse-output/wikipedia-depth-1 ../tse-output/wikipedia-index-1
