DEF_CMD(HLT, 0, 0,
    ASSERT_CODE()
    Gpu->rip = Gpu->commands_count;
    printf("stopped calculating, exititng...\n");
    ASSERT_GPU(Gpu);
)

DEF_CMD(IN, 1, 0,
    ASSERT_CODE()
    type_t value = 0;
    printf("value in: ");
    scanf("%g", &value);
    Push(&Gpu->stack, value);
    ASSERT_GPU(Gpu);
)

DEF_CMD(OUT, 2, 0,
    ASSERT_CODE()
    GET_VALUE()

    printf("value out: ");
    printf("%g\n", value);

    ASSERT_GPU(Gpu);
)

DEF_CMD(ADD, 3, 0,
    ASSERT_CODE()

    GET_RHS()
    GET_LHS()
    PUSH_OPERATION(+);
    ASSERT_GPU(Gpu);
)

DEF_CMD(SUB, 4, 0,
    ASSERT_CODE()

    GET_RHS()
    GET_LHS()

    PUSH_OPERATION(-);

    ASSERT_GPU(Gpu);
)

DEF_CMD(MUL, 5, 0,
    ASSERT_CODE()

    GET_RHS()
    GET_LHS()

    PUSH_OPERATION(*);

    ASSERT_GPU(Gpu);
)

DEF_CMD(DIV, 6, 0,
    ASSERT_CODE()

    GET_RHS()
    GET_LHS()

    PUSH_OPERATION(/);

    ASSERT_GPU(Gpu);
)

DEF_CMD(SQR, 7, 0,
    ASSERT_CODE()

    GET_VALUE()

    Push(&Gpu->stack, value * value);
    ASSERT_GPU(Gpu);
)

DEF_CMD(SQRT, 8, 0,
    ASSERT_CODE()

    GET_VALUE()

    PUSH_FUNC(sqrt)

    ASSERT_GPU(Gpu);
)

DEF_CMD(SIN, 9, 0,
    ASSERT_CODE()

    GET_VALUE()

    PUSH_FUNC(sin)

    ASSERT_GPU(Gpu);
)

DEF_CMD(COS, 10, 0,
    ASSERT_CODE()

    GET_VALUE()

    PUSH_FUNC(cos)

    ASSERT_GPU(Gpu);
)

DEF_CMD(PUSH, 56, 1,
    ASSERT_CODE()

    type_t value = 0;
    value = ARGUMENT(type_t);

    Gpu->code_pos += sizeof(type_t);

    Push(&Gpu->stack, value);

    ASSERT_GPU(Gpu);
)

DEF_CMD(POP, 25, 0,
    ASSERT_CODE()

    Pop(&Gpu->stack);

    ASSERT_GPU(Gpu);
)

DEF_CMD(REG_PUSH, 88, 1,
    ASSERT_CODE()
    int register_position = Gpu->code[Gpu->code_pos++];

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
)

DEF_CMD(REG_POP, 89, 1,
    ASSERT_CODE()

    int register_position = Gpu->code[Gpu->code_pos++];

    GET_VALUE()

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
)


DEF_CMD(JMP, 42, 1,
    ASSERT_CODE()
    int jump_pos = ARGUMENT(int);

    Gpu->code_pos = jump_pos;
    ASSERT_GPU(Gpu);
)

DEF_CMD(JA, 58, 1,
    ASSERT_CODE()
    GET_RHS()
    GET_LHS()
    GET_JMP_POS()

    COMP_AND_JMP(>)
    ASSERT_GPU(Gpu);
)

DEF_CMD(JAE, 59, 1,
    ASSERT_CODE()
    GET_RHS()
    GET_LHS()
    GET_JMP_POS()

    COMP_AND_JMP(>=)

    ASSERT_GPU(Gpu);
)

DEF_CMD(JB, 74, 1,
    ASSERT_CODE()
    GET_RHS()
    GET_LHS()
    GET_JMP_POS()

    COMP_AND_JMP(<)

    ASSERT_GPU(Gpu);
)

DEF_CMD(JBE, 75, 1,
    ASSERT_CODE()
    GET_RHS()
    GET_LHS()
    GET_JMP_POS()

    COMP_AND_JMP(<=)

    ASSERT_GPU(Gpu);
)

DEF_CMD(JE, 90, 1,
    ASSERT_CODE()
    GET_RHS()
    GET_LHS()
    GET_JMP_POS()

    COMP_AND_JMP(==)

    ASSERT_GPU(Gpu);
)

DEF_CMD(JNE, 91, 1,
    ASSERT_CODE()
    GET_RHS()
    GET_LHS()
    GET_JMP_POS()

    COMP_AND_JMP(!=)

    ASSERT_GPU(Gpu);
)

DEF_CMD(CALL, 106, 1,
    ASSERT_CODE()
    int jump_pos = ARGUMENT(int);

    Gpu->ret_positions[Gpu->current_ret] = Gpu->code_pos + sizeof(int);
    Gpu->current_ret++;
    Gpu->code_pos = jump_pos;
    ASSERT_GPU(Gpu);
)

DEF_CMD(RET, 107, 1,
    ASSERT_CODE()
    Gpu->code_pos = Gpu->ret_positions[Gpu->current_ret - 1];
    Gpu->current_ret--;
    ASSERT_GPU(Gpu);
)

DEF_CMD(HELLO, 20, 0,
    ASSERT_CODE()

    printf("Hello, world!\n");
    ASSERT_GPU(Gpu)
)

DEF_CMD(OPERATION_PUSH, 120, 2,
    ASSERT_CODE()
    int register_position = Gpu->code[Gpu->code_pos++];

    type_t value = 0;
    value = ARGUMENT(type_t);
    Gpu->code_pos += sizeof(type_t);

    switch (register_position) {

        case RAX: Push(&Gpu->stack, Gpu->rax + value);
            break;

        case RBX: Push(&Gpu->stack, Gpu->rbx + value);
            break;

        case RCX: Push(&Gpu->stack, Gpu->rcx + value);
            break;

        case RDX: Push(&Gpu->stack, Gpu->rdx + value);
            break;

        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu)
)

DEF_CMD(RAM_PUSH, 184, 1,
    ASSERT_CODE()

    int index = (int)ARGUMENT(type_t);
    Gpu->code_pos += sizeof(int);

    Push(&Gpu->stack, PopRam(Gpu, index));

    ASSERT_GPU(Gpu)
)

DEF_CMD(RAM_OPERATION_PUSH, 248, 3,
    ASSERT_CODE()

    int register_position = Gpu->code[Gpu->code_pos++];

    int index = (int)ARGUMENT(type_t);
    Gpu->code_pos += sizeof(int);

    switch (register_position) {

        case RAX: Push(&Gpu->stack, PopRam(Gpu, Gpu->rax + index));
            break;

        case RBX: Push(&Gpu->stack, PopRam(Gpu, Gpu->rbx + index));
            break;

        case RCX: Push(&Gpu->stack, PopRam(Gpu, Gpu->rcx + index));
            break;

        case RDX: Push(&Gpu->stack, PopRam(Gpu, Gpu->rdx + index));
            break;

        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu)
)

DEF_CMD(RAM_REG_PUSH, 216, 1,
    ASSERT_CODE()
    int register_position = Gpu->code[Gpu->code_pos++];

    switch (register_position) {

        case RAX: Push(&Gpu->stack, PopRam(Gpu, (int)Gpu->rax));
            break;

        case RBX: Push(&Gpu->stack, PopRam(Gpu, (int)Gpu->rbx));
            break;

        case RCX: Push(&Gpu->stack, PopRam(Gpu, (int)Gpu->rcx));
            break;

        case RDX: Push(&Gpu->stack, PopRam(Gpu, (int)Gpu->rdx));
            break;

        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu)
)


DEF_CMD(RAM_POP, 185, 1,
    ASSERT_CODE()

    int index = (int)ARGUMENT(type_t);
    Gpu->code_pos += sizeof(int);

    GET_VALUE()

    PushRam(Gpu, index, value);

    ASSERT_GPU(Gpu)
)

DEF_CMD(RAM_OPERATION_POP, 249, 3,
    ASSERT_CODE()

    int register_position = Gpu->code[Gpu->code_pos++];

    int index = (int)ARGUMENT(type_t);
    Gpu->code_pos += sizeof(int);

    GET_VALUE()

    switch (register_position) {

        case RAX: PushRam(Gpu, Gpu->rax + index, value);
            break;

        case RBX: PushRam(Gpu, Gpu->rbx + index, value);
            break;

        case RCX: PushRam(Gpu, Gpu->rcx + index, value);
            break;

        case RDX: PushRam(Gpu, Gpu->rdx + index, value);
            break;

        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu)
)

DEF_CMD(RAM_REG_POP, 217, 1,
    ASSERT_CODE()
    int register_position = Gpu->code[Gpu->code_pos++];

    GET_VALUE()

    switch (register_position) {

        case RAX: PushRam(Gpu, (int)Gpu->rax, value);
            break;

        case RBX: PushRam(Gpu, (int)Gpu->rbx, value);
            break;

        case RCX: PushRam(Gpu, (int)Gpu->rcx, value);
            break;

        case RDX: PushRam(Gpu, (int)Gpu->rdx, value);
            break;

        default : fprintf(stderr,"Register with pos <%d> doesn't exist", register_position);
    }

    ASSERT_GPU(Gpu)
)

DEF_CMD(KERN, 40, 0,
    ASSERT_CODE()

    int book_size = 1000000;
    char* book = (char*) calloc (book_size, sizeof(char));
    FILE* fp_book = fopen("KerniganAndRitchie.txt", "r");
    book_size = fread(book, 1, book_size, fp_book);
    fwrite(book, 1, book_size, stdout);
    printf("\n");
    fclose(fp_book);
    free(book);

    ASSERT_GPU(Gpu)
)
