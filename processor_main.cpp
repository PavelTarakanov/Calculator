#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "stack.h"
#include "processor.h"

int main(int argc, char* argv[])
{
    processor_t processor = {};
    FILE* input_address = NULL;
    int number_of_commands = 0;

    if (check_file_founded(argc))
        return FILE_NOT_FOUND_ERROR;

    if (check_file_opening(argv[1], &input_address, "r"))
        return FILE_OPENING_ERROR;

    if (read_programm(input_address, &processor.programm , &number_of_commands))
        return FILE_READIND_ERROR;

    if (check_file_closing(input_address))
        return FILE_CLOSING_ERROR;

    if (processor_init(&processor, START_STACK_SIZE))
        return INITIALISATION_ERROR;

    calculator(&processor, number_of_commands);

    //video_memory(processor.RAM);

    cleaner(&processor);

    return NO_FILE_ERROR;
}
