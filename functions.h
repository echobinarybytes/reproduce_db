#ifndef __FUNCTIONS_H_

#define __FUNCTIONS_H_
#include "db_types.h"


/******* page related functions  *********/

extern void * get_page(Pager *pager, uint32_t page_num);

extern Pager* pager_open(const char *filename); 

/******* db related functions (include open db and close db) *********/

extern void db_close(Table *table);
extern Table * db_open(const char *filename)

/******* sql compiler related functions  *********/

MetaCommandResult do_meta_command(InputBuffer * input_buffer); 

/* insert operation */
extern PrepareResult prepare_insert(InputBuffer *input_buffer, Statement *statement);

/* select operation */
extern PrepareResult prepare_select(InputBuffer *input_buffer, Statement *statement);

/* allocate memory for storing input statement*/
extern InputBuffer *new_input_buffer();

/* read_input statement */
extern void read_input(InputBuffer *input_buffer);

/* prepare statement for execute*/
extern PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement);

/* execute statement */
extern void execute_statement(Statement * statement);

/* close allocated memory by new_input_buffer() function */
void close_input_buffer(InputBuffer * input_buffer) {



#endif
