#ifndef STACK_H
#define STACK_H

const int NUMBER_OF_FILES = 2;

const int START_STACK_SIZE = 10;
const int LEFT_BIRD = 2147483647;
const int RIGTH_BIRD = 2147483646;

enum File_Usimng_Error_Code
{
    NO_FILE_ERROR = 0,
    FILE_NOT_FOUND_ERROR = 1,
    FILE_OPENING_ERROR = 2,
    FILE_CLOSING_ERROR = 3,
};

enum Stack_Error_Code
{
    NO_ERROR = 0,
    STK_ADDRESS_ERROR = 1,
    STK_SIZE_LESS_ZERO_ERROR = 2,
    LEFT_BIRD_ERROR = 3,
    RIGTH_BIRD_ERROR = 4,
};

struct stack_t{int* data;
               int size;
               int capacity;};

struct processor_t{stack_t stk;
                 int* programm;
                 int instruction_pointer;
};

bool check_file_founded(int argc, const char *input_address);
bool check_file_opening(const char *input_address, FILE** file_ptr);
bool check_file_closing(FILE* input_address);

int read_text(FILE* input_address, char** buffer);
void make_mashine_code(FILE* output_address, char* buffer);

bool StackInit(stack_t* stk, int capacity);
void stack_upgrade(stack_t* stk);
Stack_Error_Code StackPush(stack_t* stk, int value);
Stack_Error_Code StackPop(stack_t* stk, int* address);
Stack_Error_Code StackVerify(stack_t* stk);
void StackDump(stack_t* stk);

bool read_programm(FILE* input_address, int** programm, int* number_of_commands);
void calculator(stack_t* stk, int* programm);
bool do_user_command(int command, stack_t* stk, int* command_number, int* programm);

int my_strcmp(const char* str_1, const char* str_2);

#endif //STACK_H
