#include "Stack.h"
#include "Calc.h"
#include "Commands.h"

void Calculate(struct StackArray* stack, FILE* fp_in) {
    int command_no = -1;
    fscanf(fp_in, "%d", &command_no);

    while (!feof(fp_in)) {

        switch (command_no) {

            case HLT: Hlt_(stack);
                break;

            case IN: In_(stack);
                break;

            case OUT: Out_(stack);
                break;

            case ADD: Add_(stack);
                break;

            case SUB: Sub_(stack);
                break;

            case MUL: Mul_(stack);
                break;

            case DIV: Div_(stack);
                break;

            case SQR: Sqr_(stack);
                break;

            case SQRT: Sqrt_(stack);
                break;

            case SIN: Sin_(stack);
                break;

            case COS: Cos_(stack);
                break;

            case PUSH: Push_(stack, fp_in);
                break;

            case POP: Pop_(stack);
                break;

            default : printf("Unknown command\n");

        }

        fscanf(fp_in, "%d", &command_no);
    }

}

int main() {
    struct StackArray stack;
    Construct(0, &stack);

    printf("file in: ");
    scanf("%s", file_in);
    FILE* fp_in = fopen(file_in, "r");
    assert(fp_in != NULL);

    Calculate(&stack, fp_in);

    fclose(fp_in);
 }

void Hlt_(struct StackArray* stack) {
    assert(stack != NULL);

    Destroy(stack);

    printf("stopped calculating, exititng...\n");
    exit(EXIT_SUCCESS);
}

void In_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t value = 0;
    printf("value in: ");
    scanf("%g", &value);

    Push(stack, value);
}

void Out_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t value = 0;
    Top(stack, &value);
    Pop(stack);

    printf("value out: ");
    printf("%g\n", value);
}

void Push_(struct StackArray* stack, FILE* fp_in) {
    assert(stack != NULL);

    type_t value = 0;
    fscanf(fp_in, "%g", &value);

    Push(stack, value);
}

void Pop_(struct StackArray* stack) {
    assert(stack != NULL);

    Pop(stack);
}

void Add_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(stack, &lhs_value);
    Pop(stack);
    Top(stack, &rhs_value);
    Pop(stack);

    Push(stack, lhs_value + rhs_value);
}

void Div_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(stack, &lhs_value);
    Pop(stack);
    Top(stack, &rhs_value);
    Pop(stack);

    Push(stack, lhs_value / rhs_value);
}

void Mul_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(stack, &lhs_value);
    Pop(stack);
    Top(stack, &rhs_value);
    Pop(stack);

    Push(stack, lhs_value * rhs_value);

}

void Sub_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t lhs_value = 0;
    type_t rhs_value = 0;
    Top(stack, &lhs_value);
    Pop(stack);
    Top(stack, &rhs_value);
    Pop(stack);

    Push(stack, lhs_value - rhs_value);
}

void Sqr_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t value = 0;
    Top(stack, &value);
    Pop(stack);

    value = value * value;

    Push(stack, value);
}

void Sqrt_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t value = 0;
    Top(stack, &value);
    Pop(stack);

    value = sqrt(value);

    Push(stack, value);
}

void Cos_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t value = 0;
    Top(stack, &value);
    Pop(stack);

    value = cos(value);

    Push(stack, value);
}

void Sin_(struct StackArray* stack) {
    assert(stack != NULL);

    type_t value = 0;
    Top(stack, &value);
    Pop(stack);

    value = sin(value);

    Push(stack, value);
}


