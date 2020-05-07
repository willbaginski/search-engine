#!/bin/bash

# compile crawler
make clean
make

# begin testing. Directory ./testdir will be used
mkdir ./testdir

# test several cases with incorrect command-line arguments
./crawler
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./testdir number
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./testdir -1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./nonexistent 2

# test case where server is nonexistent
./crawler ?? ./testdir 2

# test case where server is non-internal
./crawler http://wikipedia.com ./testdir 2

# test case where server is valid but page is nonexistent
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/fakepage.html ./testdir 2

# ./testdir should be empty other than .crawler

# test case with closed set of cross linked pages at various depths. Output to specified directories
mkdir ./test1dir0
mkdir ./test1dir1
mkdir ./test1dir2
mkdir ./test1dir3
mkdir ./test1dir4
mkdir ./test1dir5
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1dir0 0
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1dir1 1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1dir2 2
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1dir3 3
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1dir4 4
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1dir5 5

# test on wikipedia at depths 0, 1
mkdir ./wiki0
mkdir ./wiki1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./wiki0 0
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./wiki1 1

# test on playground
mkdir ./playground
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./playground 3
