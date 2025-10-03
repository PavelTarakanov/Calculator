#ifndef STACK_H
#define STACK_H

const int NUMBER_OF_FILES = 2;

const int START_STACK_SIZE = 10;
const int LEFT_BIRD = 2147483647;
const int RIGTH_BIRD = 2147483646;

enum Error_Code
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

int check_file_founded(int argc, const char *input_address);
int check_file_opening(const char *input_address, FILE** file_ptr);
int check_file_closing(FILE* input_address);

int read_text(FILE* input_address, char** buffer);
void do_mashine_code(FILE* output_address, char* buffer);

void StackInit(stack_t* stk, int capacity);
void stack_upgrade(stack_t* stk);
Error_Code StackPush(stack_t* stk, int value);
Error_Code StackPop(stack_t* stk, int* address);
Error_Code StackVerify(stack_t* stk);
void StackDump(stack_t* stk);

void calculator(stack_t* stk, FILE* input_address);
bool do_user_command(int command, stack_t* stk, FILE* input_address);

int my_strcmp(const char* str_1, const char* str_2);

#endif //STACK_H
