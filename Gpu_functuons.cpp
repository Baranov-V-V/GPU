#include "Calc.h"

void Hlt_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    Gpu->rip = Gpu->commands_count;

    printf("stopped calculating, exititng...\n");

    ASSERT_GPU(Gpu);
}

void In_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    printf("value in: ");
    scanf("%g", &value);

    Push(&Gpu->stack, value);

    ASSERT_GPU(Gpu);
}

void Out_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    Top(&Gpu->stack, &value);
    Pop(&Gpu->stack);

    printf("value out: ");
    printf("%g\n", value);

    ASSERT_GPU(Gpu);
}

void Push_(struct GPU* Gpu) { //переделать
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    value = *(type_t*) (Gpu->code + Gpu->code_pos);
    Gpu->code_pos += sizeof(type_t);

    Push(&Gpu->stack, value);

    ASSERT_GPU(Gpu);
}

void Pop_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    Pop(&Gpu->stack);

    ASSERT_GPU(Gpu);
}

void Add_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(&Gpu->stack, &lhs_value);
    Pop(&Gpu->stack);
    Top(&Gpu->stack, &rhs_value);
    Pop(&Gpu->stack);

    Push(&Gpu->stack, lhs_value + rhs_value);

    ASSERT_GPU(Gpu);
}

void Div_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(&Gpu->stack, &lhs_value);
    Pop(&Gpu->stack);
    Top(&Gpu->stack, &rhs_value);
    Pop(&Gpu->stack);

    Push(&Gpu->stack, lhs_value / rhs_value);

    ASSERT_GPU(Gpu);
}

void Mul_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(&Gpu->stack, &lhs_value);
    Pop(&Gpu->stack);
    Top(&Gpu->stack, &rhs_value);
    Pop(&Gpu->stack);

    Gpu->commands_count = -100;

    Push(&Gpu->stack, lhs_value * rhs_value);

    ASSERT_GPU(Gpu);
}

void Sub_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(&Gpu->stack, &lhs_value);
    Pop(&Gpu->stack);
    Top(&Gpu->stack, &rhs_value);
    Pop(&Gpu->stack);

    Push(&Gpu->stack, lhs_value - rhs_value);

    ASSERT_GPU(Gpu);
}

void Sqr_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    Top(&Gpu->stack, &value);
    Pop(&Gpu->stack);

    value = value * value;

    Push(&Gpu->stack, value);

    ASSERT_GPU(Gpu);
}

void Sqrt_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    Top(&Gpu->stack, &value);
    Pop(&Gpu->stack);

    value = sqrt(value);

    Push(&Gpu->stack, value);

    ASSERT_GPU(Gpu);
}

void Cos_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    Top(&Gpu->stack, &value);
    Pop(&Gpu->stack);

    Push(&Gpu->stack, cos(value));

    ASSERT_GPU(Gpu);
}

void Sin_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    type_t value = 0;
    Top(&Gpu->stack, &value);
    Pop(&Gpu->stack);

    Push(&Gpu->stack, sin(value));

    ASSERT_GPU(Gpu);
}

void RegPush_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    int register_position = Gpu->code[Gpu->code_pos++];

    printf("pushed from register %d\n", register_position);    //убрать

    switch (register_position) {

        case RAX: Push(&Gpu->stack, Gpu->rax);
            break;

        case RBX: Push(&Gpu->stack, Gpu->rbx);
            break;

        case RCX: Push(&Gpu->stack, Gpu->rcx);
            break;

        case RDX: Push(&Gpu->stack, Gpu->rdx);
            break;

        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu);
}

void RegPop_(struct GPU* Gpu) {
    assert(Gpu != NULL);
    ASSERT_GPU(Gpu);

    int register_position = Gpu->code[Gpu->code_pos++];

    type_t value = 0;
    Top(&Gpu->stack, &value);
    Pop(&Gpu->stack);

    printf("poped to register %d value %g\n",register_position, value);  //убрать

    switch (register_position) {

        case RAX: Gpu->rax = value;
            break;

        case RBX: Gpu->rbx = value;
            break;

        case RCX: Gpu->rcx = value;
            break;

        case RDX: Gpu->rdx = value;
            break;

        default : fprintf(stderr,"Syntax error: register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu);
}
