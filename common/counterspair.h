/*
 * counterspair.h -- header file for counterspair module
 *
 * a structure that contains two counters
 *
 * Will Baginski, May 2020
 */

#include "../libcs50/counters.h"

typedef struct counterspair counterspair_t;

/*****functions*****/

// creates and returns a pointers to a new counterspair
// takes two counters as arguments
// returns NULL if allocation fails
counterspair_t *counterspair_new(counters_t *one, counters_t *two);

// getter methods for the two counters
counters_t *counterspair_getOne(counterspair_t *pair);
counters_t *counterspair_getTwo(counterspair_t *pair);

// doesn't need a special delete function
