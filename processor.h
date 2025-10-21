#ifndef PROCESSOR_H
#define PROCESSOR_H

const int RAM_SIZE = 5000;
const int NUMBER_OF_REGS = 16;

enum Commands
{
    HLT = -1,
    POP = 0,
    PUSH = 1,
    OUT = 2,
    ADD = 3,
    MUL = 4,
    SUB = 5,
    DIV = 6,
    IN = 7,
    CALL = 8,
    RET = 9,
    SQRT = 10,
    POPR = 42,
    POPM = 43,
    PUSHR = 33,
    PUSHM = 34,
    JB = 50,
    VIDEO = 51,
    PAUSE = 52,
};

enum File_Usimng_Error_Code
{
    NO_FILE_ERROR = 0,
    FILE_NOT_FOUND_ERROR = 1,
    FILE_OPENING_ERROR = 2,
    FILE_CLOSING_ERROR = 3,
    FILE_READIND_ERROR = 4,
    INITIALISATION_ERROR = 5,
    UNKNOWN_COMMAND = 6,
    MAKING_MASHINE_CODE_ERROR = 7
};

struct processor_t{stack_t stk;
                   stack_t ret_stk;
                   int* programm;
                   int* regs;
                   int instruction_pointer;
                   int* RAM;
};

Stack_Error_Code processor_init(processor_t* processor, const unsigned int capacity);
void processor_dump(processor_t* processor, int number_of_commands);
void video_memory(int* RAM);

bool read_programm(FILE* input_address, int** programm, int* number_of_commands);
void calculator(processor_t* processor, int number_of_commands);
bool do_user_command(int command, processor_t* processor);

void cleaner(processor_t* processor);

#endif //PROCESSOR_H
