#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>//TODO прыгает куда-то не туда
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "stack.h"

int main(int argc, char* argv[])
{
    processor_t processor = {};
    FILE* input_address = NULL;
    int number_of_commands = 0;

    if (check_file_founded(argc, argv[0]))
        return FILE_NOT_FOUND_ERROR;

    if (check_file_opening(argv[1], &input_address))
        return FILE_OPENING_ERROR;

    if (read_programm(input_address, &processor.programm , &number_of_commands))
        return ALLOCATION_ERROR;

    if (check_file_closing(input_address))
        return FILE_CLOSING_ERROR;
    /*
    for (int i = 0; i < number_of_commands*2; i++)
    {
        getchar();
        printf("%d\n", processor.programm[i]);
    }
    */
    if (processor_init(&processor, START_STACK_SIZE))
        return INITIALISATION_ERROR;

    //processor_dump(&processor, number_of_commands);

    calculator(&processor);

    //StackDump(&stk);
    cleaner(&processor);

    return NO_FILE_ERROR;
}

Stack_Error_Code processor_init(processor_t* processor, const unsigned int capacity)
{
    assert(processor);

    if (stack_init(&processor->stk, capacity))
        return ALLOCATION_ERROR;

    if (stack_init(&processor->ret_stk, capacity))
        return ALLOCATION_ERROR;

    processor->regs = (int*) calloc(NUMBER_OF_REGS, sizeof(int));
    if (processor->regs == NULL)
        return ALLOCATION_ERROR;

    processor->instruction_pointer = 0;

    return NO_ERROR;
}

void processor_dump(processor_t* processor, int number_of_commands)
{
    printf("processor[%p]\n"
           "programm:\n", processor);
    for (int i = 0; i < number_of_commands; i++)
    {
        printf("    [%d] = %d", i, processor->programm[i]);
        if (i == processor->instruction_pointer)
            printf("<--");
        printf("\n");
    }
    printf("regs:\n");
    for (int i = 0; i < 16; i++)
    {
        printf("    regs[%d] = %d\n", i, processor->regs[i]);
    }
    stack_dump(&processor->stk);
}

bool read_programm(FILE* input_address, int** programm, int* number_of_commands)
{
    assert(input_address);
    assert(programm);
    assert(number_of_commands);

    int command = -2;

    fscanf(input_address, "%d", number_of_commands);
    *programm = (int*) calloc(*number_of_commands, sizeof(int));

    if (*programm == NULL)
    {
        return 1;
    }

    for (int i = 0; i < *number_of_commands; i++)
    {
        fscanf(input_address, "%d", &command);
        //printf("%d\n", command);
        (*programm)[i] = command;
    }

    return 0;
}

void calculator(processor_t* processor)
{
    assert(processor);

    int command = processor->programm[processor->instruction_pointer];

    //printf("%s %d", command, value);
    while (command)
    {
        //printf("%d\n", command);
        if (do_user_command(command, processor))
            break;

        //processor_dump(processor, number_of_commands);
        //getchar();


        /*
        stack_dump(&processor->stk);
        printf("%d", processor->programm[processor->instruction_pointer]);
        getchar();*/

        if (stack_verify(&processor->stk))
        {
            printf("ERROR!\nStack_Error_Code - %d\n", stack_verify(&processor->stk));
            break;
        }
        processor->instruction_pointer++;
        command = processor->programm[processor->instruction_pointer];
        //printf("%d\n", command);
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

            stack_push(&processor->stk, value);

            return 0;
        case 2:
            stack_pop(&processor->stk, &value);

            printf("%d\n", value);

            return 0;
        case 3:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_1+elem_2);

            return 0;
        case 4:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_1*elem_2);

            return 0;
        case 5:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_2-elem_1);

            return 0;
        case 6:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);
            stack_push(&processor->stk, elem_2/elem_1);

            return 0;
        case 7:
            scanf("%d", &value);
            stack_push(&processor->stk, value);

            return 0;
        case 8:
            (processor->instruction_pointer)++;
            stack_push(&processor->ret_stk, processor->instruction_pointer);
            //processor->regs[15] = processor->instruction_pointer;
            processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;

            return 0;
        case 9:
            stack_pop(&processor->ret_stk, &processor->instruction_pointer);

            return 0;
        case 10:
            stack_pop(&processor->stk, &elem_1);
            stack_push(&processor->stk, (int) sqrt(elem_1));

            return 0;
        case 42:
            stack_pop(&processor->stk, &value);

            (processor->instruction_pointer)++;
            reg_number = processor->programm[processor->instruction_pointer];
            (processor->regs)[reg_number] = value;
            //printf("POPR %d", value);

            return 0;
        case 33:
            (processor->instruction_pointer)++;
            reg_number = processor->programm[processor->instruction_pointer];

            stack_push(&processor->stk, (processor->regs)[reg_number]);

            return 0;
        case 50:
            stack_pop(&processor->stk, &elem_1);
            stack_pop(&processor->stk, &elem_2);

            (processor->instruction_pointer)++;
            if (elem_1>elem_2)
                processor->instruction_pointer = processor->programm[processor->instruction_pointer] - 1;
            return 0;

        default:
            printf("Print real command!\n");
            return 1;
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

void cleaner(processor_t* processor)
{
    free(processor->stk.data);
    free(processor->ret_stk.data);

}
