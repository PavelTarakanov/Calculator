#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "stack.h"

int main(int argc, char* argv[])
{
    stack_t stk = {};
    processor_t processor = {.stk = stk, .instruction_pointer = 0};
    FILE* input_address = NULL;
    int number_of_commands = 0;

    if (check_file_founded(argc, argv[0]))
        return FILE_NOT_FOUND_ERROR;

    if (check_file_opening(argv[1], &input_address))
        return FILE_OPENING_ERROR;

    read_programm(input_address, &processor.programm , &number_of_commands);

    if (check_file_closing(input_address))
        return FILE_CLOSING_ERROR;

    /*
    for (int i = 0; i < number_of_commands*2; i++)
    {
        getchar();
        printf("%d\n", programm[i]);
    }
    */
    if (StackInit(&processor.stk, START_STACK_SIZE))
        return 0;

    calculator(&processor.stk, processor.programm);

    //StackDump(&stk);

    return 0;
}

bool read_programm(FILE* input_address, int** programm, int* number_of_commands)//TODO bool?
{
    int command = -1;
    int i = 0;

    fscanf(input_address, "%d", number_of_commands);
    *programm = (int*) calloc(*number_of_commands*2, sizeof(int));

    while (command != 0)
    {
        fscanf(input_address, "%d", &command);
        (*programm)[i] = command;
        i++;
    }

    *number_of_commands = i;

    return 1;
}

void calculator(stack_t* stk, int* programm)//TODO
{
    assert(stk);
    assert(programm);

    int command_number = 0;
    int command = programm[command_number];

    //printf("%s %d", command, value);
    while (command)
    {
        if (do_user_command(command, stk, &command_number, programm))
            break;

        //StackDump(stk);

        if (StackVerify(stk))
        {
            printf("ERROR!\nStack_Error_Code - %d\n", StackVerify(stk));
            break;
        }

        command_number++;
        command = programm[command_number];
    }

    return;
}

bool do_user_command(int command, stack_t* stk, int* command_number, int* programm)
{
    assert(stk);
    assert(command_number);
    assert(programm);

    int value = 0;
    int elem_1 = 0;
    int elem_2 = 0;

    switch(command)
    {
        case 0:
            return 1;
        case 1:
           (*command_number)++;
            value = programm[*command_number];

            StackPush(stk, value);

            return 0;
        case 2:
            StackPop(stk, &value);

            printf("%d\n", value);

            return 0;
        case 3:
            StackPop(stk, &elem_1);
            StackPop(stk, &elem_2);
            StackPush(stk, elem_1+elem_2);

            return 0;
        case 4:
            StackPop(stk, &elem_1);
            StackPop(stk, &elem_2);
            StackPush(stk, elem_1*elem_2);

            return 0;
        case 5:
            StackPop(stk, &elem_1);
            StackPop(stk, &elem_2);
            StackPush(stk, elem_2-elem_1);

            return 0;
        case 6:
            StackPop(stk, &elem_1);
            StackPop(stk, &elem_2);
            StackPush(stk, elem_2/elem_1);

            return 0;
        case 7:
            StackPop(stk, &elem_1);
            StackPush(stk, (int) sqrt(elem_1));

            return 0;
        default:
            printf("Print real command!\n");
            return 0;
    }
}

int my_strcmp(const char* str_1, const char* str_2)
{
    assert(str_1);
    assert(str_2);

    int i = 0;
    int j = 0;

    while (str_1[i] != '\n' && str_2[j] != '\n' && str_1[i] != '\0' && str_2[i] != '\0')
    {
        if (str_1[i] > str_2[j])
        {
            return 1;
        }
        else if(str_1[i] < str_2[j])
        {
            return -1;
        }

        i++;
        j++;
    }

    return 0;
}
