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
    long long int number_of_symbols = 0;

    if (check_file_founded(argc, argv[0]))
        return FILE_NOT_FOUND_ERROR;
    if (check_file_opening(argv[1], &input_address))
        return FILE_OPENING_ERROR;
    if (check_file_opening(argv[2], &output_address))
        return FILE_OPENING_ERROR;

    number_of_symbols = read_text(input_address, &buffer);

    if (!number_of_symbols)
    {
        printf("Error while reading from file\n");
        return 0;
    }

    make_mashine_code(output_address, buffer, number_of_symbols);

    if (check_file_closing(input_address))
        return FILE_CLOSING_ERROR;
    if (check_file_closing(output_address))
        return FILE_CLOSING_ERROR;

    return 0;
}

long long int read_text(FILE* input_address, char** buffer)
{
    assert(input_address);

    struct stat statistics = {};
    long long int number_of_symbols = 0;

    fstat(fileno(input_address), &statistics);

    //printf("%ld\n", statistics.st_size);

    *buffer = (char*) calloc(statistics.st_size, sizeof(char));
    number_of_symbols =  fread(*buffer, sizeof(char), statistics.st_size, input_address);

    /*
    for (long long int i = 0; i < number_of_symbols; i++)
    {
        putchar((*buffer)[i]);
    }
    */
    return number_of_symbols;
}

void make_mashine_code(FILE* output_address, char* buffer, long long int number_of_symbols)
{
    assert(output_address);
    assert(buffer);

    int start_number_of_command = 10;
    int start_number_of_labels = 10;
    int* mashine_code = (int*) calloc(start_number_of_command, sizeof(int));
    int* labels = (int*) calloc(start_number_of_labels, sizeof(int));
    bool error = 0;

    make_code_massive(buffer, &mashine_code, &labels, &start_number_of_command, &start_number_of_labels, number_of_symbols);
    make_code_massive(buffer, &mashine_code, &labels, &start_number_of_command, &start_number_of_labels, number_of_symbols);

    if (!error)
    {
        for (int i = 0; i < mashine_code[0]+1; i++)
        {
            fprintf(output_address, "%d ", mashine_code[i]);
            //printf("%d ", mashine_code[i]);
            //getchar();
        }
        fprintf(output_address, "%d", -1);
    }

    free(mashine_code);
    free(labels);

    return;
}

bool make_code_massive(char* buffer, int** mashine_code, int** labels, int* start_number_of_command,
                               int* start_number_of_labels, long long int number_of_symbols)
{
    assert(buffer);
    assert(mashine_code);
    assert(labels);
    assert(start_number_of_command);
    assert(start_number_of_labels);

    char command[COMMAND_SIZE] = {};
    int number_of_command = 1;
    int number_of_label = -1;//TODO number_of_str
    bool error = 0;
    char* buffer_end = buffer + number_of_symbols*sizeof(char);

    sscanf(buffer,"%s", command);
    while(buffer < buffer_end)
    {
        //printf("cmd = '%s'\n", command);
        massive_upgrade(mashine_code, start_number_of_command, number_of_command);

        if (command[0] == ';')
            number_of_command--;
        else if (command[0] == ':')
        {
            number_of_command--;
            sscanf(command+1, "%d", &number_of_label);
            label_upgrade(labels, start_number_of_labels, number_of_label);
            (*labels)[number_of_label] = number_of_command;

        }
        else if (strcmp(command, "HLT") == 0)
        {
            (*mashine_code)[number_of_command] = HLT;
        }
        else if (strcmp(command, "POP") == 0)
        {
            (*mashine_code)[number_of_command] = POP;
        }
        else if (strcmp(command, "PUSH") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            //printf("Write 1\n");
            (*mashine_code)[number_of_command] = PUSH;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%d", &value);
            sscanf(buffer, "%s", command);
            //printf("%d\n", number_of_command);
            (*mashine_code)[number_of_command] = value;
        }
        else if (strcmp(command, "OUT") == 0)
        {
            (*mashine_code)[number_of_command] = OUT;
        }
        else if (strcmp(command, "ADD") == 0)
        {
            (*mashine_code)[number_of_command] = ADD;
        }
        else if (strcmp(command, "MUL") == 0)
        {
            (*mashine_code)[number_of_command] = MUL;
        }
        else if (strcmp(command, "SUB") == 0)
        {
            (*mashine_code)[number_of_command] = SUB;
        }
        else if (strcmp(command, "DIV") == 0)
        {
            (*mashine_code)[number_of_command] = DIV;
        }
        else if (strcmp(command, "IN") == 0)
        {
            (*mashine_code)[number_of_command] = IN;
        }
        else if (strcmp(command, "RET") == 0)
        {
            (*mashine_code)[number_of_command] = RET;
        }
        else if (strcmp(command, "SQRT") == 0)
        {
            (*mashine_code)[number_of_command] = SQRT;
        }
        else if (strcmp(command, "POPR") == 0)//TODO new function
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            (*mashine_code)[number_of_command] = POPR;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%s", command);

            value = command[1] - 'A';//TODO специальная функция для реальных названиц регистров
            (*mashine_code)[number_of_command] = value;
        }
        else if (strcmp(command, "PUSHR") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            (*mashine_code)[number_of_command] = PUSHR;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%s", command);

            value = command[1] - 'A';

            (*mashine_code)[number_of_command] = value;
        }
        else if (strcmp(command, "JB") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            //printf("Write 1\n");
            (*mashine_code)[number_of_command] = JB;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%d", &value);
            sscanf(buffer, "%s", command);
            if (command[0] == ':')
            {
                sscanf(command+1, "%d", &number_of_label);
                label_upgrade(labels, start_number_of_labels, number_of_label);
                (*mashine_code)[number_of_command] = (*labels)[number_of_label];
            }
            else
            {
                (*mashine_code)[number_of_command] = value;
            }
        }
        else if (strcmp(command, "CALL") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            //printf("Write 1\n");
            (*mashine_code)[number_of_command] = CALL;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%d", &value);
            sscanf(buffer, "%s", command);
            if (command[0] == ':')
            {
                sscanf(command+1, "%d", &number_of_label);
                label_upgrade(labels, start_number_of_labels, number_of_label);
                (*mashine_code)[number_of_command] = (*labels)[number_of_label];
            }
            else
            {
                (*mashine_code)[number_of_command] = value;
            }
        }
        else if (strcmp(command, "POPM") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            (*mashine_code)[number_of_command] = POPM;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%s", command);

            value = command[2] - 'A';

            if (command[0] != '[' || command[strlen(command)-1] != ']')
            {
                printf("asm.asm:%d: syntax_error\n", number_of_command);
                error = 1;
            }

            (*mashine_code)[number_of_command] = value;
        }
        else if (strcmp(command, "PUSHM") == 0)
        {
            int value = 0;

            buffer += (strlen(command)+1)*sizeof(char);
            (*mashine_code)[number_of_command] = PUSHM;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);
            number_of_command++;
            massive_upgrade(mashine_code, start_number_of_command, number_of_command);

            sscanf(buffer, "%s", command);

            value = command[2] - 'A';
            if (command[0] != '[' || command[strlen(command)-1] != ']')
            {
                printf("asm.asm:%d: syntax_error\n", number_of_command);
                error = 1;
            }

            (*mashine_code)[number_of_command] = value;
        }
        else
        {
            printf("asm.asm:%d: syntax_error\n", number_of_command);
            error = 1;
        }

        number_of_command++;
        buffer += (strlen(command)+1)*sizeof(char);
        sscanf(buffer,"%s", command);
    }

    number_of_command--;
    (*mashine_code)[number_of_command] = -1;
    (*mashine_code)[0] = number_of_command;

    return error;
}

void massive_upgrade(int** mashine_code, int* start_number_of_command, int number_of_command)
{
    assert(mashine_code);
    assert(start_number_of_command);

    if (number_of_command >= *start_number_of_command)
    {
        *start_number_of_command *= 2;
        //printf("%p %d\n", *mashine_code, *start_number_of_command);
        *mashine_code = (int*) realloc(*mashine_code, (*start_number_of_command)*sizeof(int));
    }
}

void label_upgrade(int** labels, int* start_number_of_labels, int number_of_label)
{
    assert(labels);
    assert(start_number_of_labels);

    if (number_of_label >= *start_number_of_labels)
    {
        *start_number_of_labels = number_of_label + 1;
        //printf("%p %d\n", *mashine_code, *start_number_of_command);
        *labels = (int*) realloc(*labels, (*start_number_of_labels)*sizeof(int));
    }

    return;
}
