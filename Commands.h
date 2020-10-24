#pragma once

#include "Stack.h"

const int COMMANDS_COUNT = 15;
const int MAX_COMMAND_LEN = 5;

typedef float type_t;
typedef uint64_t gpu_canary_t;

const gpu_canary_t GPU_CANARY = 0xBAAADCFEFEBDCBFE;

/*
enum Commands {
    HLT      = 0,
    IN       = 1,
    OUT      = 2,
    ADD      = 3,
    SUB      = 4,
    MUL      = 5,
    DIV      = 6,
    SQR      = 7,
    SQRT     = 8,
    SIN      = 9,
    COS      = 10,
    PUSH     = 11,
    POP      = 12,
    REG_PUSH = 27,
    REG_POP  = 28,
};
*/

enum Commands {
    #define DEF_CMD(name_upper, name_lower, num, arg, code) \
    name_upper = num,

    #include "Code_Gen_Gpu.h"

    #undef DEF_CMD
};

static const Command codificator[] {
    #define DEF_CMD(name_upper, name_lower, num, arg, code) \
    {#name_lower, name_upper},

    #include "Code_Gen_Gpu.h"

    #undef DEF_CMD
};


enum Registers {
    RAX = 1,
    RBX = 2,
    RCX = 3,
    RDX = 4,
};

struct GPU {
    gpu_canary_t canary_begin;

    struct StackArray stack;

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

    gpu_canary_t canary_end;
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


struct FileHeader {
    char initials[3];
    char version;
    int commands_count;
    int code_size;
};

typedef struct Command {
    char name[MAX_COMMAND_LEN];
    enum Commands code;
} Command;

static const Command codificator[] {
    #define DEF_CMD(name_upper, name_lower, num, arg, code) \
    {#name_lower, name_upper},

    #include "Code_Gen_Gpu.h"

    #undef DEF_CMD
};

/*
static const Command codificator[] {
    {"hlt", HLT},
    {"in", IN},
    {"out", OUT},
    {"add", ADD},
    {"sub", SUB},
    {"mul", MUL},
    {"div", DIV},
    {"sqr", SQR},
    {"sqrt", SQRT},
    {"sin", SIN},
    {"cos", COS},
    {"push", PUSH},
    {"pop", POP},
    {"push", REG_PUSH},
    {"pop", REG_POP},
};
*/
