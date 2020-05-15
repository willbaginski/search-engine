/*
 * word -- contains a function used to normalzie a word. See word.h for more detail
 *
 * Will Baginski, May 2020
 */

#include <string.h>
#include <ctype.h>

// converts a word to lowercase
void NormalizeWord(char *word) {
	for (int i = 0; i < strlen(word); i++) {
		word[i] = tolower(word[i]);
	}
}
