#!/bin/bash

# compile indexe and indextest:
make clean
make

# test case with the wrong number of arguments:
./indexer

# test case where pageDir is not a directory created by crawler:
./indexer ../indexer/ test

# test case where file is not a path to a valid file:
./indexer ../tse-output/letters-depth-4 /

# run indexer on example crawler output to create testIndex:
./indexer ../tse-output/letters-depth-4 testIndex

# run indextest with wrong number of arguments:
./indextest

# run indextest with a nonexistent oldFile:
./indextest fake testCopy

# run indextest on testIndex to create testCopy:
./indextest testIndex testCopy

# organize the two files with indexsort.awk (nothing should be printed):
gawk -f indexsort.awk testIndex > testIndex.sorted
gawk -f indexsort.awk testCopy > testCopy.sorted

# compare the two files (if they are identical nothing should be printed):
diff testIndex.sorted testCopy.sorted
