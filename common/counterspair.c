/*
 * counterspair.c
 *
 * counterspair.h contains further info
 *
 * Will Baginski, May 2020
 */

#include <stdio.h>
#include <stdlib.h>

#include "counterspair.h"
#include "../libcs50/counters.h"

// define the structure
typedef struct counterspair {
	// pair of counters
	counters_t *one;
	counters_t *two;
} counterspair_t;

/*****global functions*****/
counterspair_t *counterspair_new(counters_t *one, counters_t *two) {
	counterspair_t *pair = malloc(sizeof(counterspair_t));
	if (pair == NULL) {
		return NULL;
	} else {
		// initialize structure
		pair->one = one;
		pair->two = two;
		return pair;
	}
}

counters_t *counterspair_getOne(counterspair_t *pair) {
	if (pair == NULL) {
		return NULL;
	} else {
		return pair->one;
	}
}

counters_t *counterspair_getTwo(counterspair_t *pair) {
	if (pair == NULL) {
		return NULL;
	} else {
		return pair-> two;
	}
}
