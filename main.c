#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "db_types.h"
#include "functions.h"


/*
 *  Main Entry 
 */
int main(int argc, char* argv[]) {
    /*
    if (argc < 2) {
        printf("must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];
    */

    /* obtain filename by hard code for convinence*/
    char *filename = "./db_test.db";
    Table *table = db_open(filename);
    InputBuffer * input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
                case (META_COMMAND_SUCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_NEGATIVE_ID):
                printf("ID must be positive.\n");
                break;
            case (PREPARE_STRING_TOO_LONG):
                printf("String is too long\n");
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("unrecognized key word at start of '%s'\n", input_buffer->buffer);
                continue; // ? what's the difference between break and continue here
        }

        execute_statement(&statement);
        printf("executed.\n");

    }
    return 0;
}
