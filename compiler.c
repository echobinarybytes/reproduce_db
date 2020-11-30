#include "db_types.h"
#include "functions.h"


/* * a wrapper for existing functionality that leaves more room for more command
 */
MetaCommandResult do_meta_command(InputBuffer * input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        db_close(table);
        exit(EXIT_SUCCESS);
    }
}

/*
 * action of command "insert"
 */
PrepareResult prepare_insert(InputBuffer *input_buffer, Statement *statement) {

    statement->type = STATEMENT_INSERT;
    /* parse every part of input command */
    char *keyword = strtok(input_buffer->buffer, "");
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");

    /* check the every part right or wrong */
    if (id_string == NULL || username==NULL || email==NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(id_string);
    if (id<0) {
        return PREPARE_NEGATIVE_ID;
    }
    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    if (strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    /* assign value to correspond part of statement */
    statement->row_to_insert.id = id;
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);

    return PREPARE_SUCCESS;
}

/*
 * action of command insert
 */
PrepareResult prepare_select(InputBuffer *input_buffer, Statement *statement) {
    return PREPARE_SUCCESS;
}

/*
 * sql compiler
 * parse command
 */
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement) {
    if (strncmp(inputbuffer->buffer, "insert", 6) == 0) {
        return prepare_insert(input_buffer, statement);
    }

    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return prepare_select(inputbuffer, statement);
    }
}
/*
 *
 */
void execute_statement(Statement * statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            printf("this is where we would do an insert.\n");
            break;
        case (STATEMENT_SELECT):
            printf("this is where we do a select.\n");
            break;
    }
}
/*
 * allocate space for InputBuffer structure
 */
InputBuffer *new_input_buffer() {
    InputBuffer * input_buffer = (InputBuffer*) malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length - 0;
    return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);
    if (bytes_read <=0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read-1] = 0;
}

void close_input_buffer(InputBuffer * input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}
