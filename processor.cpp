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
    processor_t processor = {.stk = stk, .regs = (int*) calloc(16, sizeof(int)), .instruction_pointer = 0};
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
        printf("%d\n", processor.programm[i]);
    }
    */
    if (StackInit(&processor.stk, START_STACK_SIZE))
        return 0;

    calculator(&processor);

    //StackDump(&stk);

    return 0;
}

bool read_programm(FILE* input_address, int** programm, int* number_of_commands)//TODO bool?
{
    assert(input_address);
    assert(programm);
    assert(number_of_commands);

    int command = -2;
    int i = 0;

    fscanf(input_address, "%d", number_of_commands);
    *programm = (int*) calloc(*number_of_commands*2, sizeof(int));

    while (command != -1)
    {
        fscanf(input_address, "%d", &command);
        (*programm)[i] = command;
        i++;
    }

    *number_of_commands = i;

    return 1;
}

void calculator(processor_t* processor)//TODO
{
    assert(processor);

    int command = processor->programm[processor->instruction_pointer];

    //printf("%s %d", command, value);
    while (command)
    {
        if (do_user_command(command, processor))
            break;

        //StackDump(stk);

        if (StackVerify(&processor->stk))
        {
            printf("ERROR!\nStack_Error_Code - %d\n", StackVerify(&processor->stk));
            break;
        }

        processor->instruction_pointer++;
        command = processor->programm[processor->instruction_pointer];
    }

    return;
}

bool do_user_command(int command, processor_t* processor)
{
    assert(processor);

    int value = 0;
    int reg_number = -1;
    int elem_1 = 0;
    int elem_2 = 0;

    switch(command)
    {
        case -1:
            return 1;
        case 1:
            (processor->instruction_pointer)++;
            value = processor->programm[processor->instruction_pointer];

            StackPush(&processor->stk, value);

            return 0;
        case 2:
            StackPop(&processor->stk, &value);

            printf("%d\n", value);

            return 0;
        case 3:
            StackPop(&processor->stk, &elem_1);
            StackPop(&processor->stk, &elem_2);
            StackPush(&processor->stk, elem_1+elem_2);

            return 0;
        case 4:
            StackPop(&processor->stk, &elem_1);
            StackPop(&processor->stk, &elem_2);
            StackPush(&processor->stk, elem_1*elem_2);

            return 0;
        case 5:
            StackPop(&processor->stk, &elem_1);
            StackPop(&processor->stk, &elem_2);
            StackPush(&processor->stk, elem_2-elem_1);

            return 0;
        case 6:
            StackPop(&processor->stk, &elem_1);
            StackPop(&processor->stk, &elem_2);
            StackPush(&processor->stk, elem_2/elem_1);

            return 0;
        case 7:
            StackPop(&processor->stk, &elem_1);
            StackPush(&processor->stk, (int) sqrt(elem_1));

            return 0;
        case 42:
            StackPop(&processor->stk, &value);

            (processor->instruction_pointer)++;
            reg_number = processor->programm[processor->instruction_pointer];
            (processor->regs)[reg_number] = value;

            return 0;
        case 33:
            (processor->instruction_pointer)++;
            reg_number = processor->programm[processor->instruction_pointer];

            StackPush(&processor->stk, (processor->regs)[reg_number]);

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
