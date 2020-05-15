/*
 * pagedir - save webpages
 *
 * Will Baginski, April 2020
 */

#include <stdio.h>
#include <string.h>

#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

// saves page contents to a file named ID within directory/
// formatted with the first line being the url, the second being the depth,
// and the rest of the content being the page's html
void page_saver(webpage_t *page, char *directory, int ID) {
	char filename[100];
	// create the filename, accounting for possibly formats of directory argument
	if (directory[strlen(directory) - 1] != '/') {
		sprintf(filename, "%s/%d", directory, ID);
	} else {
		sprintf(filename, "%s%d", directory, ID);
	}
	// create/open the file
	FILE *fp;
	fp = fopen(filename, "w");
	
	// write the url, then the depth, then the html
	fprintf(fp, "%s\n", webpage_getURL(page));
	fprintf(fp, "%d\n", webpage_getDepth(page));
	fprintf(fp, "%s", webpage_getHTML(page));
	
	// clean up our mess and prepare for the next call of the function
	fclose(fp);
}

// constructs and returns a webpage from a file created by crawler
webpage_t *page_load(FILE *fp) {
	// first line is the url
	char *url = freadlinep(fp);
	
	// second line is the depth -- read it, then scan it as an integer
	char *depth_c = freadlinep(fp);
	int depth;
	sscanf(depth_c, "%d", &depth);
	free(depth_c);

	// rest of the file is the html
	char *html = freadfilep(fp);
	
	// if the construction fails, NULL will be returned
	return webpage_new(url, depth, html);
}
