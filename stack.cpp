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
    FILE* input_address = NULL;

    if (check_file_founded(argc, argv[0]))
        return 0;

    if (check_file_opening(argv[1], &input_address))
        return 0;

    StackInit(&stk, START_STACK_SIZE);

    calculator(&stk, input_address);

    //StackDump(&stk);

    if (check_file_closing(input_address))
        return 0;

    return 0;
}

void StackInit(stack_t* stk, int capacity)
{
    stk->size = 1;
    stk->capacity = capacity;
    stk->data = (int*) calloc(capacity + 2, sizeof(int*));
    stk->data[0] = LEFT_BIRD;
    stk->data[capacity+1] = RIGTH_BIRD;
    //StackDump(stk);

    return;
}

Error_Code StackPush(stack_t* stk, int value)
{
    Error_Code err = StackVerify(stk);

    if (err)
    {
        StackDump(stk);
    }

    if (stk->size == stk->capacity)
        stack_upgrade(stk);

    stk->data[stk->size++] = value;

    err = StackVerify(stk);

    if (err)
    {
        StackDump(stk);
    }

    return err;
}

Error_Code StackPop(stack_t* stk, int* address)
{
    Error_Code err = StackVerify(stk);

    if (err)
    {
        StackDump(stk);
    }

    *address = stk->data[--stk->size];
    stk->data[stk->size] = 0;

    err = StackVerify(stk);

    if (err)
    {
        StackDump(stk);
    }

    return err;
}

Error_Code StackVerify(stack_t* stk)
{
    if (stk == NULL)
        return STK_ADDRESS_ERROR;

    if (stk->size <= 0)
        return STK_SIZE_LESS_ZERO_ERROR;

    if (stk->data[0] != LEFT_BIRD)
        return LEFT_BIRD_ERROR;

    if (stk->data[stk->capacity+1] != RIGTH_BIRD)
        return RIGTH_BIRD_ERROR;

    return NO_ERROR;
}

void StackDump(stack_t* stk)
{
    printf("stack [%p]\n"
            "{\n"
            "   size = %d\n"
            "   capasity = %d\n"
            "   data[%p]\n"
            "   {\n"
            , stk, stk->size, stk->capacity, stk->data);
    for (int i = 0; i < stk->capacity+2; i++)
    {
        if (stk->data[i] != 0)
            printf("        *[%d] = %d\n", i, stk->data[i]);

        else
            printf("         [%d] = %d\n", i, stk->data[i]);
    }
    printf("    }\n"
           "}\n");

    return;
}

void calculator(stack_t* stk, FILE* input_address)
{
    int command = -1;

    fscanf(input_address, "%d", &command);
    //printf("%s %d", command, value);
    while (command)
    {
        if (do_user_command(command, stk, input_address))
            break;

        //StackDump(stk);

        if (StackVerify(stk))
        {
            printf("ERROR!\nError_Code - %d\n", StackVerify(stk));
            break;
        }
        fscanf(input_address, "%d", &command);
    }

    return;
}
bool do_user_command(int command, stack_t* stk, FILE* input_address)
{
    if (command == 1)
    {
        int value = 0;

        fscanf(input_address, "%d", &value);

        StackPush(stk, value);

        return 0;
    }

    if (command == 2)
    {
        int value = 0;

        StackPop(stk, &value);

        printf("%d\n", value);

        return 0;
    }

    if (command == 3)
    {
        int elem_1 = 0;
        int elem_2 = 0;

        StackPop(stk, &elem_1);
        StackPop(stk, &elem_2);
        StackPush(stk, elem_1+elem_2);

        return 0;
    }

    if (command == 4)
    {
        int elem_1 = 0;
        int elem_2 = 0;

        StackPop(stk, &elem_1);
        StackPop(stk, &elem_2);
        StackPush(stk, elem_1*elem_2);

        return 0;
    }

    if (command == 5)
    {
        int elem_1 = 0;
        int elem_2 = 0;

        StackPop(stk, &elem_1);
        StackPop(stk, &elem_2);
        StackPush(stk, elem_2-elem_1);

        return 0;
    }

    if (command == 6)
    {
        int elem_1 = 0;
        int elem_2 = 0;

        StackPop(stk, &elem_1);
        StackPop(stk, &elem_2);
        StackPush(stk, elem_2/elem_1);

        return 0;
    }

    if (command == 7)
    {
        int elem_1 = 0;

        StackPop(stk, &elem_1);
        StackPush(stk, (int) sqrt(elem_1));

        return 0;
    }

    if (command == 0)
    {
        return 1;
    }

    else
    {
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
        if (!isalpha(str_1[i]))
        {
            i++;
            continue;
        }
        else if (!isalpha(str_2[j]))
        {
            j++;
            continue;
        }
        else if (tolower(str_1[i]) > tolower(str_2[j]))
        {
            return 1;
        }
        else if(tolower(str_1[i]) < tolower(str_2[j]))
        {
            return -1;
        }

        i++;
        j++;
    }

    return 0;
}

void stack_upgrade(stack_t* stk)
{
    stk->data[stk->capacity+1] = 0;
    stk->capacity = stk->capacity*2;
    stk->data = (int*) realloc(stk->data, (stk->capacity + 2)*sizeof(int));
    stk->data[stk->capacity+1] = RIGTH_BIRD;
}
