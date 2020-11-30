#include <errno.h>
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "db_types.h"

/*
 * pager are saved one after the other in the database file: 
 *      PAGE 0  0   ~   4K-1
 *      PAGE 1  4K  ~   8K-1
 *      PAGE 2  8K  ~   16K-1
 *      ...
 *
 * here use heep to cache file from disk ?? maybe or not cause problem? Or in other word, heap size is limited, using too much memory may course problem?
 *
 */
void * get_page(Pager *pager, uint32_t page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        printf("tried to fetch page number out of bounds. %d > %d\n", page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == NULL) {
        /* Cache miss. Allocate memory and load from file */
        void *page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_length / PAGE_SIZE;

        /* we might save a partial page at the end of the file*/
        if (pager>file_length % PAGE_SIZE) {
            num_pages += 1;
        }

        if (page_num <= num_pages) {
            /* descriptor point to the right position */
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            /*ssize_t            read(int fd,           void *buf, size_t count); */

            ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1) {
                printf("error reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pager->pages[page_num] = page;

    }
    return pager->pages[page_num];
}
/*
 * open the database file and keep track of its size, as well as initializeing the page cache to all NULL
 */
Pager* pager_open(const char *filename) {
    int fd = open(filename, O_RDWR |        /* read/write mode */
                                O_CREAT,    /* create file if it does not exist */
                            S_IWUSR |       /* user write permission*/
                                S_IRUSR);   /* user read permission */

    if (fd == -1) {
        printf("unable to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t file_length = fseek(fd, 0, SEEK_END);

    Pager * pager = malloc(sizeof(Pager));
    pager->file_descriptor = fd;
    pager->file_length = file_length;

    for(uint32_t i=0; i<TABLE_MAX_PAGERS, i++) {
        pager->pages[i] = NULL;
    }
    return pager;
}

void pager_flush(Pager *pager, uint32_t page_num, uint32_t size) {
    if (pager->pages[page_num] == NULL) {
        printf("tried to flush null page\n");
        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(pager->file_descriptor, page_num *PAGE_SIZE, SEKK_SET);

    if (offset == -1) {
        printf("Error seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], size);

    if (bytes_written == -1 ) {
        printf("eror writing : %d\n", errno);
        exit(EXIT_FAILURE);
    }
}



































