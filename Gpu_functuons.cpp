#include "Calc.h"

#define ASSERT_CODE()       \
    assert(Gpu != NULL);    \
    ASSERT_GPU(Gpu);

#define GET_LHS()                 \
    type_t lhs_value = 0;         \
    Top(&Gpu->stack, &lhs_value); \
    Pop(&Gpu->stack);

#define GET_RHS()                 \
    type_t rhs_value = 0;         \
    Top(&Gpu->stack, &rhs_value); \
    Pop(&Gpu->stack);

#define GET_JMP_POS()                \
    int jump_pos = ARGUMENT(int);    \
    Gpu->code_pos += sizeof(int);

#define ARGUMENT(type)                     \
    *(type*) (Gpu->code + Gpu->code_pos)   \

#define PUSH_OPERATION(operand)            \
    Push(&Gpu->stack, lhs_value operand rhs_value);

#define PUSH_FUNC(func)                \
    Push(&Gpu->stack, func(value));

#define COMP_AND_JMP(comp)             \
    if (lhs_value comp rhs_value) {    \
        Gpu->code_pos = jump_pos;      \
    }

#define GET_VALUE()               \
    type_t value = 0;             \
    Top(&Gpu->stack, &value);     \
    Pop(&Gpu->stack);

#define DEF_REG(reg_name_upper, reg_name_lower, reg_num, reg_code)       \
    case reg_name_upper: Gpu->reg_name_lower = value;                    \
    break;


#define DEF_CMD(name_upper, num, arg, code)         \
    ExitCodes name_upper##_(struct GPU* Gpu) {      \
        code                                        \
        return EXIT_OK;                             \
    }

#include "Code_Gen_Gpu.h"

#undef DEF_CMD

#undef DEF_REG

//задержка для работы с оперативной памятью
void Sleep() {
    for(int i = 0; i < 100; ++i);
}

type_t PopRam(struct GPU* Gpu, int index) {
    assert(Gpu != NULL);
    assert(index >= 0);

    //Sleep();
    return Gpu->ram[index];
}

void PushRam(struct GPU* Gpu, int index, type_t value) {
    assert(Gpu != NULL);
    assert(index >= 0);

    //fprintf(stderr, "value: %g\n", value);

    //Sleep();

    if (index >= RAM_SIZE) {
        printf("invalid index: %d\n", index);
    }
    assert(index < RAM_SIZE);

    Gpu->ram[index] = value;
}


