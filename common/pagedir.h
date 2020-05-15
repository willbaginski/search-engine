/* 
 * Header file for pagedir
 * pagedir contains a function that will save the contents of a webpage to a file
 * 
 * Will Baginski, May 2020
 */

#include <stdio.h>
#include "../libcs50/webpage.h"

/* PAGE SAVER
 * outputs a page to the appropriate file
 */
void page_saver(webpage_t *page, char *directory, int ID);

/* PAGE LOADER
 * loads a complete webpage from a crawler output file
 * will need to be deleted with webpage_delete
 * returns NULL if the word fails
 */
webpage_t *page_load(FILE *fp);
