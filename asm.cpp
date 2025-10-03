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
        return 0;
    if (check_file_opening(argv[1], &input_address))
        return 0;
    if (check_file_opening(argv[2], &output_address))
        return 0;

    if (read_text(input_address, &buffer))
    {
        printf("Error while reading from file\n");
        return 0;
    }

    do_mashine_code(output_address, buffer);

    if (check_file_closing(input_address))
        return 0;
    if (check_file_closing(output_address))
        return 0;

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

void do_mashine_code(FILE* output_address, char* buffer)
{
    assert(output_address);
    assert(buffer);

    char command[10] = {};
    int number_of_str = 1;
    sscanf(buffer,"%s", command);

    while(strcmp(command, "HLT") != 0)
    {
        //printf("cmd = '%s'\n", command);

        if (strcmp(command, "PUSH") == 0)
        {
            buffer += (strlen(command)+1)*sizeof(char);
            //printf("Write 1\n");
            fprintf(output_address, "1");

            sscanf(buffer, "%s", command);

            fprintf(output_address, " %s", command);
        }
        else if (strcmp(command, "OUT") == 0)
        {
            fprintf(output_address, "2");
        }
        else if (strcmp(command, "ADD") == 0)
        {
            fprintf(output_address, "3");
        }
        else if (strcmp(command, "MUL") == 0)
        {
            fprintf(output_address, "4");
        }
        else if (strcmp(command, "SUB") == 0)
        {
            fprintf(output_address, "5");
        }
        else if (strcmp(command, "DIV") == 0)
        {
            fprintf(output_address, "6");
        }
        else
        {
            printf("asm.asm:%d: syntax_error\n", number_of_str);//TODO спросить нужно ли сохранаять в отдельный буффер а только потом вводить в отдельный файл
        }

        number_of_str++;
        buffer += (strlen(command)+1)*sizeof(char);
        sscanf(buffer,"%s", command);
        fprintf(output_address, " \n");
    }

    fprintf(output_address, "0");

    return;
}

