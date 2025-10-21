#ifndef ASM_H
#define ASM_H

const int COMMAND_SIZE = 200;

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


long long int read_text(FILE* input_address, char** buffer);
bool massive_upgrade(int** mashine_code, int* start_number_of_command, int number_of_command);
bool label_upgrade(int** labels, int* start_number_of_labels, int number_of_label);
bool make_mashine_code(FILE* output_address, char* buffer, long long int number_of_symbols);
bool make_code_massive(char* buffer, int** mashine_code, int** labels, int* start_number_of_command,
                               int* start_number_of_labels, long long int number_of_symbols);

int reg_number(const char* reg);

bool do_push_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_popr_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_pushr_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_jb_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_call_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer,
                  int** labels, int* start_number_of_labels, int number_of_label);
bool do_popm_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);
bool do_pushm_code(char* command,int** mashine_code, int** start_number_of_command,
                  int* number_of_command, char** buffer);

#endif //ASM_H
