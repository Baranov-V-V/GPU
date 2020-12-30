#pragma once

#include "Stack.h"
#include <stdint.h>

enum CommandsCounter {
    #define DEF_CMD(name_upper, num, arg, code) \
    COUNT_##name_upper,

    #include "Code_Gen_Gpu.h"

    #undef DEF_CMD
    COMMANDS_COUNT
};

//const int COMMANDS_COUNT = MEO + 1;
const int MAX_COMMAND_LEN = 20;
const int MAX_LABEL_LENGTH = 20;
const int MAX_LABELS_COUNT = 1000;
const int MAX_RECURSION = 1000;
const int RAM_SIZE = 10000;
const int BEGIN_JMP_CODES = 15;
const int END_JMP_CODES = 22;

typedef float type_t;
typedef uint64_t gpu_canary_t;

const gpu_canary_t GPU_CANARY = 0xBAAADCFEFEBDCBFE;


enum Commands {
    #define DEF_CMD(name_upper, num, arg, code) \
    COMMAND_##name_upper = num,

    #include "Code_Gen_Gpu.h"

    #undef DEF_CMD
};

typedef struct Command {
    char name[MAX_COMMAND_LEN];
    enum Commands code;
} Command;

static const Command codificator[] {
    #define DEF_CMD(name_upper, num, arg, code) \
    {#name_upper, COMMAND_##name_upper},

    #include "Code_Gen_Gpu.h"

    #undef DEF_CMD
};


enum Registers {
    #define DEF_REG(name_upper, name_lower, num, code) \
        name_upper = num,

    #include "Registers_Gen.h"

    #undef DEF_REG
};

struct GPU {
    gpu_canary_t canary_begin;
    struct StackArray stack;
    gpu_canary_t canary_end;

    char version;

    char* code;
    int rip;
    int commands_count;
    int code_pos;
    int code_size;

    type_t rax; // 1
    type_t rbx; // 2
    type_t rcx; // 3
    type_t rdx; // 4

    int* ret_positions;
    int current_ret;

    type_t* ram;

};

struct Push_values {
    char rx_type;
    type_t value;
};

enum GpuErrors {
    GPU_OK                  = 0,
    CODE_POS_ERROR          = 1,
    COMMANDS_COUNT_ERROR    = 2,
    RIP_ERROR               = 3,
    CODE_NULL               = 4,
    COMMAND_OVERFLOW        = 5,
    BEGIN_CODE_CANARY_ERROR = 6,
    END_CODE_CANARY_ERROR   = 7,
    BEGIN_GPU_CANARY_ERROR  = 8,
    END_GPU_CANARY_ERROR    = 9,
    RAX_VALUE_ERROR         = 10,
    RBX_VALUE_ERROR         = 11,
    RCX_VALUE_ERROR         = 12,
    RDX_VALUE_ERROR         = 13,
    CODE_SIZE_ERROR         = 14,
};

static const char* GpuErrorNames[] = {
    "OK",
    "CODE POSITION IS INVALID",
    "COMMANDS COUNT IS INVALID",
    "COMMAND COUNTER ERROR",
    "CODE IS NULL",
    "COMMANDS OVERFLOW",
    "LEFT CODE CANARY VALUE IS CHANGED",
    "RIGHT CODE CANARY VALUE IS CHANGED",
    "LEFT GPU CANARY VALUE IS CHANGED",
    "RIGHT GPU CANARY VALUE IS CHANGED",
    "RAX VALUE IS INVALID",
    "RBX VALUE IS INVALID",
    "RCX VALUE IS INVALID",
    "RDX VALUE IS INVALID"
};

enum ExitCodes {
    EXIT_OK = 0,
};

struct FileHeader {
    char initials[3];
    char version;
    int commands_count;
    int code_size;
};

typedef struct Label {
    int pos;
    char name[MAX_LABEL_LENGTH];
} Label;
