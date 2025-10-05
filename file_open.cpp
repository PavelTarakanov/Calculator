#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

bool check_file_founded(int argc, const char *input_address)
{
    if (argc < NUMBER_OF_FILES)
    {
        fprintf(stderr, "Using %s input_address\n", input_address);
        return 1;
    }

    return 0;
}

bool check_file_opening(const char *input_address, FILE** file_ptr)
{
    if ((*file_ptr = fopen(input_address, "a+")) == NULL)
    {
        fprintf(stderr, "Can't open file\" %s\"\n", input_address);
        return 1;
    }

    return 0;
}

bool check_file_closing(FILE* input_address)
{
    assert(input_address);

    if (fclose(input_address) != 0)
    {
        fprintf(stderr, "Error while closing file\n");
        return 1;
    }

    return 0;
}

