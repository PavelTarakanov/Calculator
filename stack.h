#ifndef STACK_H
#define STACK_H//TODO two headers

const int COMMAND_SIZE = 200;
const int NUMBER_OF_REGS = 16;
const unsigned int START_STACK_SIZE = 10;
const int LEFT_BIRD = 2147483647;
const int RIGTH_BIRD = 2147483646;
const int RAM_SIZE = 100;

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
};

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
    STK_SIZE_MORE_CAPACITY_ERROR = 3,
    LEFT_BIRD_ERROR = 4,
    RIGTH_BIRD_ERROR = 5,
    ALLOCATION_ERROR = 6,
    REALLOCATION_ERROR = 7
};

struct stack_t{int* data;
               unsigned int size;
               unsigned int capacity;};

struct processor_t{stack_t stk;
                   stack_t ret_stk;
                   int* programm;
                   int* regs;
                   int instruction_pointer;
                   int* RAM;
};

bool check_file_founded(int argc, const char *input_address);
bool check_file_opening(const char *input_address, FILE** file_ptr);
bool check_file_closing(FILE* input_address);

long long int read_text(FILE* input_address, char** buffer);
void massive_upgrade(int** mashine_code, int* start_number_of_command, int number_of_command);
void make_mashine_code(FILE* output_address, char* buffer, long long int number_of_symbols);
bool make_code_massive(char* buffer, int** mashine_code, int** labels, int* start_number_of_command,
                               int* start_number_of_labels, long long int number_of_symbols);

Stack_Error_Code stack_init(stack_t* stk, unsigned int capacity);
Stack_Error_Code processor_init(processor_t* processor, const unsigned int capacity);
void processor_dump(processor_t* processor, int number_of_commands);
void video_memory(int* RAM);

Stack_Error_Code stack_upgrade(stack_t* stk);
Stack_Error_Code stack_degrade(stack_t* stk);
Stack_Error_Code stack_push(stack_t* stk, int value);
Stack_Error_Code stack_pop(stack_t* stk, int* address);
Stack_Error_Code stack_verify(stack_t* stk);
void stack_dump(stack_t* stk);

bool read_programm(FILE* input_address, int** programm, int* number_of_commands);
void calculator(processor_t* processor, int number_of_commands);
bool do_user_command(int command, processor_t* processor);

void cleaner(processor_t* processor);


int my_strcmp(const char* str_1, const char* str_2);

#endif //STACK_H
