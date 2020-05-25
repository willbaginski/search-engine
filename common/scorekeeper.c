/*
 * scorekeeper.c -- details in scorekeeper.h
 *
 * Will Baginski, May 2020
 */

#include <stdlib.h>
#include <stdbool.h>

/*****global type*****/
typedef struct scorekeeper {
	int docID;
	int score;
	bool real;
} scorekeeper_t;

/*****functions*****/

scorekeeper_t *scorekeeper_new(int docID, int score) {
	scorekeeper_t *scorekeeper = malloc(2 * sizeof(int));
	if (scorekeeper == NULL) {
		return NULL;
	} else {
		scorekeeper->docID = docID;
		scorekeeper->score = score;
		return scorekeeper;
	}
}

int scorekeeper_getDocID(scorekeeper_t *scorekeeper) {
	if (scorekeeper == NULL) {
		return -1;
	} else {
		return scorekeeper->docID;
	}
}

int scorekeeper_getScore(scorekeeper_t *scorekeeper) {
	if (scorekeeper == NULL) {
		return -1;
	} else {
		return scorekeeper->score;
	}
}
