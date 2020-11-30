#ifndef __DB_TYPES_H
#define __DB_TYPES_H

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct {
    char * buffer;
    size_t buffer_length; // ?? buffer_length may smaller than input_length
    ssize_t input_length; // signed size_t
} InputBuffer;

typedef enum {
    META_COMMAND_SUCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_STRING_TOO_LONG,
    PREPAREW_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT 
} PrepareResult;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void * pages[TABLE_MAX_PAGES];
} Pager;

typedef struct {
    Pager * pager;
    uint32_t num_rows;
} Table;


typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT

} StatementType;

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
    StatementType type;
    Row row_to_insert; // only used by insert statement

} Statement;


#endif
