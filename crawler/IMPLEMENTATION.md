CS50, Lab 4
## Will Baginski, Spring 2020

### Implemenation
The crawler takes three arguments, all of which are mandatory:
* `seedURL`
* `pageDirectory`
* `maxDepth`
Requirements for them are specified in README.md

## Argument Validating
The first thing the crawler does, in main(), is make sure that the user supplied valid arguments. The following non-zero exit codes may be returned due to unnacceptable user input:
* 1: The number of arguments supplied did not equal 3.
* 2: The depth supplied could not be interpreted as a non-negative integer.
* 3: The directory supplied did not exist or is not writeable.
	This is checked with the page_validate() function. It is described with the other local functions under "Local Functions" in this file.
* 4: The url supplied is not internal.

## Preparing to Crawl
Once the arguments are confirmed to be valid, they are passed to crawler(). The main purpose of crawler() is to perform the crawling, but before it can start, it must prepare.
First, a webpage is created from the seedURL. If that webpage is NULL, the program returns exit code 5. If the webpage is valid, it continues.
Next, a bag and hashtable (200 slots) are created. The webpage created with seedURL is inserted into the bag, and the seedURL in inserted into the hashtable.
Finally, integers `ID` (to number the pages as they are extracted from the bag and `exitcode` (to keep track of errors) are created and initialized to 0, and `webpage_t *page` is declared.

## Crawling
The crawling begins. It takes place within a while loop. The while loop will continue until `page`, which is set equal to `bag_extract()`, is NULL. This ensures that the loop will run once for every webpage contained in the bag.
The first action taken on each page is an attempt to fetch its HTML using `page_fetch(page)`. If the attempt is successful, the page's url, ID, and HTML are written to a file named after the ID within `pageDirectory`. This is done using `page_saver()` from the `pagedir` module located in the `/common/` directory.
ID is then incremented. Then, if the depth of the page (obtained from `webpage_getDepth(page)` is less than `maxDepth`, the page is scanned for more urls using `page_scan(page)`.
If the attempt to fetch HTML was not successful, the exitcode is set to 6 to note the error, an error message is printed to `stderr`, and the program continues to run.
Regardless of the outcome of the fetch attempt, `page` is freed using `webpage_delete(page)` and the loop restarts.
Once the bag is empty and the while loop finished, the bag and hashtable are deleted, and the exitcode is returned.

## Local Functions
# Page Validate
Returns an integer.
This function is used to make sure that the directory supplied by the user exists and is writeable.
It first checks if the last character of the directory is '/'. If it is not, it adds '/' to ensure a consistent format for reference later on.
Then, it attempts to open `.crawler` within the directory. If the attempt is successful, no further action is taken. If it is not, it then attempts to create `.crawler` within the directory. If it is unsuccessful, the program returns 1 to signify an error to the caller. If it is successful, the program closes `.crawler` and returns 0 to signify success to the caller.

# Page Fetch
Returns a boolean.
Supplied with `webpage_t *page`, the function calls and returns the value of `webpage_fetch(page)`.

# Page Scan
Returns nothing.
Supplied with `webpage_t page`, `bag_t bag`. and `hashtable_t *ht`.
The function first declares and intializes `int pos = 0` and declares `char *url`.
The function then scans `page` for urls by entering a while loop which runs until `webpage_getNextURL(page, &pos)` is NULL.
With the url obtained in the header, the function makes sure that it is internal, adds it to the hashtable if it is, and then adds a new webpage created with the url to the bag, if the url was successfully added to the hashtable.

## Dynamic Memory Use
Dynamic memory is used when creating webpages, because `webpage_new` expects the url supplied to be dynamically allocated so that it can be fteed later. To insert only dynamically allocated urls, urls stored on the stack are copied into new variables which are dynamically allocated. Then, those variables are used as arguments in `webpage_new`.

## Error Handling
Errors are described elsewhere in this file. Generally, all errors result in the immediate termination of the program (after any necessary memory is freed) with the exception of an error due to a failed fetch attempt. When a fetch attempt fails, the program prints an error message to stderr, sets the exit code to nonzero, and continues running.
