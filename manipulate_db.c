#include <stdlib.h>
#include "db_types.h"
/*
 * close the connection to the database
 * 1. flushes the page cache to disk
 * 2. close the database file
 * 3. frees the memory for the pager and table data structures
 *
 */
void db_close(Table *table) {
    Pager * pager = table->pager;
    uint32_t num_full_pages = table->num_rows/ ROWS_PER_PAGE;

    /* find the first null page*/
    for (uint32_t i=0; i<num_full_pages; i++) {
        if (pager->pages[i] == NULL) {
            continue;
        }
    }

    /*
     * There may be a partial page to write to the end of the file 
     * a partial page means only part of the page being used.
     *
     * this should not be needed after we switch to a b-tree
     */

    uint32_t num_additional_rows = table->num_rows % ROWS_PER_PAGE;
    if (num_additional_rows > 0) {
        uint32_t page_num = num_full_pages;
        if (pager->pages[page_num] != NULL) {
            pager_flush(pager, page_num, num_additional_rows * ROW_SIZE);
            free(pager->pages[page_num]);
            pager->pages[page_num] = NULL:
        }
    }

    int result = close(pager->file_descriptor);
    if (result == -1) {
        printf("Error closing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i=0; i<TABLE_MAX_PAGES; i++) {
        void *page = pager->pages[i];
        if (page) {
            free(page);
            pager->pages[i] = NULL;
        }
    }

    free(pager);
    free(table);

}


/*
 * open a connection to the database, which means
 *  1. opening the database file
 *  2. initializing a pager data structure
 *  3. initializing a table data structure
 */
Table * db_open(const char *filename) {
    /* process 1 and 2*/
    Pager *pager = pager_open(filename); 
    uint32_t num_rows = pager->file_length / ROW_SIZE;

    /* process 3 */
    Table *table = malloc(sizeof(Table));
    table num_rows = 0;
    table->pager = pager;
    table->num_rows = num_rows;

    return table;
}
