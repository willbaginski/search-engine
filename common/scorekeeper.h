/*
 * scorekeeper.h -- header file for scorekeeper module
 *
 * scorekeeper holds a pair of ints: a docID and a score
 *
 * Will Baginski, May 2020
 */

// typedef
typedef struct scorekeeper scorekeeper_t;

// create a scorekeeper
scorekeeper_t *scorekeeper_new(int docID, int score);

// getter methods
// they return -1 if the scorekeeper is NULL, otherwise return the requested value
int scorekeeper_getDocID(scorekeeper_t *scorekeeper);
int scorekeeper_getScore(scorekeeper_t *scorekeeper);

// scorekeeper can just be freed. no need for a special delete function
