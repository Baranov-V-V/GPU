#pragma once

const int COMMANDS_COUNT = 13;
static const int MAX_COMMAND_LEN = 5;

typedef float type_t;

enum Commands {
    HLT  = 0,
    IN   = 1,
    OUT  = 2,
    ADD  = 3,
    SUB  = 4,
    MUL  = 5,
    DIV  = 6,
    SQR  = 7,
    SQRT = 8,
    SIN      = 9,
    COS      = 10,
    PUSH     = 11,
    POP      = 12,
    REG_PUSH = 110;
    REG_POP  = 120;
};

/*struct GPU {
    struct StackArray stack;

    char* code;
    int rip;

    type_t rax;
    type_t rbx;
    type_t rcx;
    type_t rdx;
};

struct FileHeader {

};*/

typedef struct Command {
    char name[MAX_COMMAND_LEN];
    enum Commands code;
} Command;

static const Command codificator[] {
    {"hlt", HLT},
    {"in", IN},
    {"out", OUT},
    {"add", ADD},
    {"sub", SUB},
    {"mul", MUL},
    {"div", DIV},
    {"sqr", SQRT},
    {"sqrt", SQR},
    {"sin", SIN},
    {"cos", COS},
    {"push", PUSH},
    {"pop", POP},
    {"push", REG_PUSH},
    {"pop", REG_POP},
};

