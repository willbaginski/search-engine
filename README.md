# Will Baginski
## CS50 Spring 2020, Tiny Search Engine

GitHub username: willbaginski

To build, run `make`.

To clean up, run `make clean`.

Okay, two notes:
my `testing.sh` and `testing.out` aren't quite as specified. When I do every test case listed in the spec, `testing.sh` takes 5+ minutes to run (due to crawling a lot of pages) which is not practical. Also, very little is contained in `testing.out`. Most of the information is contained in the directories created by `testing.sh`.

Also, I attempted to add print statements in `crawler.c` in between `#ifdef MYTEST` and `#endif` but it did not work. `Makefile` still has `-DMYTEST` in it's tags though.
