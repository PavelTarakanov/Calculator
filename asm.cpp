#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "stack.h"

int main(int argc, char* argv[])
{
    FILE* input_address = NULL;
    FILE* output_address = NULL;
    char* buffer = NULL;

    if (check_file_founded(argc, argv[0]))
        return FILE_NOT_FOUND_ERROR;
    if (check_file_opening(argv[1], &input_address))
        return FILE_OPENING_ERROR;
    if (check_file_opening(argv[2], &output_address))
        return FILE_OPENING_ERROR;

    if (read_text(input_address, &buffer))
    {
        printf("Error while reading from file\n");
        return 0;
    }

    make_mashine_code(output_address, buffer);

    if (check_file_closing(input_address))
        return FILE_CLOSING_ERROR;
    if (check_file_closing(output_address))
        return FILE_CLOSING_ERROR;

    return 0;
}

int read_text(FILE* input_address, char** buffer)
{
    assert(input_address);

    struct stat statistics = {};
    long long int number_of_symbols = 0;

    fstat(fileno(input_address), &statistics);

    //printf("%ld\n", statistics.st_size);

    *buffer = (char*) calloc(statistics.st_size, sizeof(char));
    number_of_symbols =  fread(*buffer, sizeof(char), statistics.st_size, input_address);

    if (number_of_symbols == 0 )
        return 1;
    /*
    for (long long int i = 0; i < number_of_symbols; i++)
    {
        putchar((*buffer)[i]);
    }
    */
    return 0;
}

void make_mashine_code(FILE* output_address, char* buffer)
{
    assert(output_address);
    assert(buffer);

    int start_number_of_command = 10;
    int* mashine_code = (int*) calloc(start_number_of_command, sizeof(int));
    char command[10] = {};
    int number_of_command = 1;
    int number_of_numbers = 0;
    sscanf(buffer,"%s", command);

    while(strcmp(command, "HLT") != 0)
    {
        //printf("cmd = '%s'\n", command);
        if (number_of_command >= start_number_of_command)
        {
            start_number_of_command *= 2;
            mashine_code = (int*) realloc(mashine_code, start_number_of_command*sizeof(int));
        }

        if (strcmp(command, "PUSH") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            //printf("Write 1\n");
            mashine_code[number_of_command] = 1;
            number_of_command++;
            number_of_numbers++;

            sscanf(buffer, "%d", &value);
            sscanf(buffer, "%s", command);
            mashine_code[number_of_command] = value;
        }
        else if (strcmp(command, "OUT") == 0)
        {
            mashine_code[number_of_command] = 2;
        }
        else if (strcmp(command, "ADD") == 0)
        {
            mashine_code[number_of_command] = 3;
        }
        else if (strcmp(command, "MUL") == 0)
        {
            mashine_code[number_of_command] = 4;
        }
        else if (strcmp(command, "SUB") == 0)
        {
            mashine_code[number_of_command] = 5;
        }
        else if (strcmp(command, "DIV") == 0)
        {
            mashine_code[number_of_command] = 6;
        }
        else
        {
            printf("asm.asm:%d: syntax_error\n", number_of_command);
        }

        number_of_command++;
        buffer += (strlen(command)+1)*sizeof(char);
        sscanf(buffer,"%s", command);
    }

    mashine_code[number_of_command] = 0;
    number_of_command++;
    mashine_code[0] = number_of_command - number_of_numbers - 1;

    for (int i = 0; i < number_of_command; i++)
        fprintf(output_address, "%d ", mashine_code[i]);

    return;
}

