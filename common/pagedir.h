/* 
 * Header file for pagedir
 * pagedir contains a function that will save the contents of a webpage to a file
 * 
 * Will Baginski, May 2020
 */

#include <stdio.h>
#include "webpage.h"

/* PAGE SAVER
 * outputs a page to the appropriate file
 */
void page_saver(webpage_t *page, char *directory, int ID);
