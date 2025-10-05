#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "stack.h"


bool StackInit(stack_t* stk, int capacity)
{
    assert(stk);

    stk->size = 1;
    stk->capacity = capacity;
    stk->data = (int*) calloc(capacity + 2, sizeof(int*));
    if (stk->data == NULL)
    {
        printf("Error while memory allocation\n");
        return 1;
    }
    stk->data[0] = LEFT_BIRD;
    stk->data[capacity+1] = RIGTH_BIRD;
    //StackDump(stk);

    return 0;
}

Stack_Error_Code StackPush(stack_t* stk, int value)
{
    assert(stk);

    Stack_Error_Code err = StackVerify(stk);

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

Stack_Error_Code StackPop(stack_t* stk, int* address)
{
    assert(stk);
    assert(address);

    Stack_Error_Code err = StackVerify(stk);

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

Stack_Error_Code StackVerify(stack_t* stk)
{
    assert(stk);

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
    assert(stk);

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

void stack_upgrade(stack_t* stk)
{

    stk->data[stk->capacity+1] = 0;
    stk->capacity = stk->capacity*2;
    stk->data = (int*) realloc(stk->data, (stk->capacity + 2)*sizeof(int));
    stk->data[stk->capacity+1] = RIGTH_BIRD;
}

