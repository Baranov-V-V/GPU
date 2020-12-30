#include "Calc.h"

void Calculate(struct GPU* Gpu) {
    //printf("done5");
    assert(Gpu);
    unsigned char command_no = 255;

    //txCreateWindow(900, 900, true);
    //txSetFillColor (TX_YELLOW);
   // printf("done4");
    while (true) {
        command_no = Gpu->code[Gpu->code_pos++];
      //  printf("command_no: [%hhd]", command_no);
        switch (command_no) {

        #define DEF_CMD(name_upper, num, arg, code)                          \
            case COMMAND_##name_upper: name_upper##_ (Gpu);                  \
                break;

            #include "Code_Gen_Gpu.h"

        #undef DEF_CMD

            default : printf("Unknown command with no: %d\n", command_no);
                return;
        }

        //PrintRam(Gpu);
        ++Gpu->rip;

        if (command_no == COMMAND_HLT) {
            break;
        }

    }

}

void GpuActivation(struct GPU* Gpu) {
    Construct(0, &Gpu->stack);

    Gpu->canary_begin = GPU_CANARY;
    Gpu->canary_end = GPU_CANARY;

    Gpu->rax = 0.0;
    Gpu->rbx = 0.0;
    Gpu->rcx = 0.0;
    Gpu->rdx = 0.0;

    Gpu->rip = 1;
    Gpu->commands_count = 0;
    Gpu->code = NULL;
    Gpu->code_pos = 0;
    Gpu->version = 1;

    Gpu->ret_positions = (int*) calloc(MAX_RECURSION, sizeof(int));
    Gpu->current_ret = 0;

    Gpu->ram = (type_t*) calloc(RAM_SIZE, sizeof(type_t));
}

void GpuUpdate(struct GPU* Gpu, char* file_in, FILE** fp_in) {
    assert(file_in != NULL);
    assert(Gpu != NULL);

    *fp_in = fopen(file_in, "rb");
    assert(*fp_in != NULL);

    struct stat info = {};
    stat(file_in, &info);
    int input_length = info.st_size;

    void* data_tmp = calloc(1, sizeof(char) * input_length + 2 * sizeof(gpu_canary_t));
    assert(data_tmp != NULL);

    Gpu->code = (char*)((char*)data_tmp + sizeof(gpu_canary_t));

    *(gpu_canary_t*)(Gpu->code - sizeof(gpu_canary_t)) = GPU_CANARY;
    *(gpu_canary_t*)(Gpu->code + (input_length + 1) * sizeof(char)) = GPU_CANARY;

    fread(Gpu->code, input_length, 1, *fp_in);

    FileHeader* file_header = (FileHeader*) Gpu->code;

    Gpu->commands_count = file_header->commands_count;
    Gpu->code_size = file_header->code_size;

    Gpu->code = Gpu->code + sizeof(FileHeader);

}

void GpuShutdown(struct GPU* Gpu) {

    free(Gpu->code - sizeof(FileHeader) - sizeof(gpu_canary_t));
    free(Gpu->ram);
    free(Gpu->ret_positions);

    Destroy(&Gpu->stack);

    Gpu->rax = 0;
    Gpu->rbx = 0;
    Gpu->rcx = 0;
    Gpu->rdx = 0;

    Gpu->rip = 0;
    Gpu->commands_count = 0;
    Gpu->code_pos = 0;
    Gpu->code = NULL;

}

GpuErrors GpuOk(struct GPU* Gpu) {
    assert(Gpu != NULL);

    ASSERT_OK(&(Gpu->stack));

    if (Gpu->code_pos < 0) {
        return CODE_POS_ERROR;
    }
    if (Gpu->code_size < 0) {
        return CODE_SIZE_ERROR;
    }
    if (Gpu->commands_count < 0) {
        return COMMANDS_COUNT_ERROR;
    }
    if (Gpu->rip < 0) {
        return RIP_ERROR;
    }
    if (Gpu->code == NULL) {
        return CODE_NULL;
    }

    if (*(gpu_canary_t*)(Gpu->code - sizeof(FileHeader) - sizeof(gpu_canary_t)) != GPU_CANARY) {
        return BEGIN_CODE_CANARY_ERROR;
    }
    if (*(gpu_canary_t*)(Gpu->code + (Gpu->code_size + 1) * sizeof(char)) != GPU_CANARY) {
        return END_CODE_CANARY_ERROR;
    }
    if (Gpu->canary_begin != GPU_CANARY) {
        return BEGIN_GPU_CANARY_ERROR;
    }
    if (Gpu->canary_end != GPU_CANARY) {
        return END_GPU_CANARY_ERROR;
    }

/*    if (!isfinite(Gpu->rax)) {
        return RAX_VALUE_ERROR;
    }
    if (!isfinite(Gpu->rbx)) {
        return RBX_VALUE_ERROR;
    }
    if (!isfinite(Gpu->rcx)) {
        return RCX_VALUE_ERROR;
    }
    if (!isfinite(Gpu->rdx)) {
        return RDX_VALUE_ERROR;
    }
  */
    return GPU_OK;
}

void GpuDump(struct GPU* Gpu, const char* file_name, FILE* fp, GpuErrors err_no) {
    assert(Gpu != NULL);
    assert(fp != NULL);
    assert(file_name != NULL);

    fprintf(fp, "Error code : %d (%s)\n\n", err_no, GpuErrorNames[err_no]);
    fprintf(fp, "adress of GPU: [%p]\n", Gpu);

    fprintf(fp, "{\n");

    fprintf(fp, "    version: %hhd\n", Gpu->version);
    fprintf(fp, "    current command: %d\n", Gpu->rip);
    fprintf(fp, "    commands count: %d\n", Gpu->commands_count);
    fprintf(fp, "    code adress: [%p]\n\n", Gpu->code);
    fprintf(fp, "    position in code : %d\n", Gpu->code_pos - 1);

    int dump_pos = 0;
    while (dump_pos < Gpu->code_size) {

        fprintf(fp, "    ");
        for (int i = 0; i < 8 && dump_pos + i < Gpu->code_size; ++i) {
            fprintf(fp, "%02hhX ", Gpu->code[dump_pos + i]);
        }
        fprintf(fp, "\n");

        fprintf(fp, "    ");
        for (int i = 0; i < 8 && dump_pos + i < Gpu->code_size; ++i) {
            if (dump_pos + i == Gpu->code_pos - 1 && i == 7) {
                fprintf(fp, "^-");
            }
            else if (dump_pos + i == Gpu->code_pos - 1) {
                fprintf(fp, "^--");
            }
            else if (i == 7 || dump_pos + i == Gpu->code_size - 1) {
                fprintf(fp, "--");
            }
            else {
                fprintf(fp, "---");
            }
        }
        fprintf(fp, "\n");

        dump_pos += 8;
    }
    fprintf(fp, "\n");

    fprintf(fp, "    Canary type |  Initial value   |  Current value\n");
    fprintf(fp, "    GPU begin   | %llX | %llX\n", GPU_CANARY, Gpu->canary_begin);
    fprintf(fp, "    GPU end     | %llX | %llX\n", GPU_CANARY, Gpu->canary_end);
    fprintf(fp, "    Code begin  | %llX | %llX\n", GPU_CANARY, *(gpu_canary_t*)(Gpu->code - sizeof(gpu_canary_t) - sizeof(FileHeader)));
    fprintf(fp, "    Code end    | %llX | %llX\n", GPU_CANARY, *(gpu_canary_t*)(Gpu->code + (Gpu->code_size + 1) * sizeof(char)));

    fprintf(fp, "\n");

    fprintf(fp, "    RAX value: %g\n", Gpu->rax);
    fprintf(fp, "    RBX value: %g\n", Gpu->rbx);
    fprintf(fp, "    RCX value: %g\n", Gpu->rcx);
    fprintf(fp, "    RDX value: %g\n", Gpu->rdx);

    fprintf(fp, "}\n");
}
