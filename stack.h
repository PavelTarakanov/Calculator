#ifndef STACK_H
#define STACK_H

const int NUMBER_OF_FILES = 2;

const int COMMAND_SIZE = 200;
const unsigned int START_STACK_SIZE = 10;
const int LEFT_BIRD = 2147483647;
const int RIGTH_BIRD = 2147483646;

enum File_Usimng_Error_Code
{
    NO_FILE_ERROR = 0,
    FILE_NOT_FOUND_ERROR = 1,
    FILE_OPENING_ERROR = 2,
    FILE_CLOSING_ERROR = 3,
    INITIALISATION_ERROR = 5,
    UNKNOWN_COMMAND = 6,
};

enum Stack_Error_Code
{
    NO_ERROR = 0,
    STK_ADDRESS_ERROR = 1,
    STK_SIZE_LESS_ZERO_ERROR = 2,
    LEFT_BIRD_ERROR = 3,
    RIGTH_BIRD_ERROR = 4,
    ALLOCATION_ERROR = 5,
    REALLOCATION_ERROR = 6
};

struct stack_t{int* data;
               unsigned int size;
               unsigned int capacity;};

struct processor_t{stack_t stk;
                 int* programm;
                 int* regs;
                 int instruction_pointer;
};

bool check_file_founded(int argc, const char *input_address);
bool check_file_opening(const char *input_address, FILE** file_ptr);
bool check_file_closing(FILE* input_address);

int read_text(FILE* input_address, char** buffer);
void make_mashine_code(FILE* output_address, char* buffer);
int make_code_massive(char* buffer, int number_of_command, int* labels, int** mashine_code, bool* error);

Stack_Error_Code stack_init(stack_t* stk, unsigned int capacity);
Stack_Error_Code processor_init(processor_t* processor, const unsigned int capacity);

Stack_Error_Code stack_upgrade(stack_t* stk);
Stack_Error_Code stack_push(stack_t* stk, int value);
Stack_Error_Code stack_pop(stack_t* stk, int* address);
Stack_Error_Code stack_verify(stack_t* stk);
void stack_dump(stack_t* stk);

bool read_programm(FILE* input_address, int** programm, int* number_of_commands);
void calculator(processor_t* processor);
bool do_user_command(int command, processor_t* processor);

int my_strcmp(const char* str_1, const char* str_2);

#endif //STACK_H
