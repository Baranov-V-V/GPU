#pragma once

#include "Commands.h"

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys\stat.h>

#define GPU_DUMP(Gpu, GpuErrors) {                           \
    const char* file_name = "GPU_(" #Gpu ")_Dump.txt";       \
    FILE* fp = fopen(file_name, "w");                        \
    GpuDump(Gpu, file_name, fp, GpuErrors);                  \
    fclose(fp);                                              \
}

#define ASSERT_GPU(Gpu) {                        \
    GpuErrors err_no = GpuOk(Gpu);               \
    printf("error with no: %d\n", err_no);       \
    if (err_no != GPU_OK) {                      \
        GPU_DUMP(Gpu, err_no);                   \
        assert(!"OK");                           \
    }                                            \
}


static const int MAX_NAME_LEN = 100;

static char file_in[MAX_NAME_LEN];

void Hlt_(struct GPU* Gpu);

void In_(struct GPU* Gpu);

void Out_(struct GPU* Gpu);

void Pop_(struct GPU* Gpu);

void Add_(struct GPU* Gpu);

void Div_(struct GPU* Gpu);

void Mul_(struct GPU* Gpu);

void Sub_(struct GPU* Gpu);

void Sqr_(struct GPU* Gpu);

void Sqrt_(struct GPU* Gpu);

void Cos_(struct GPU* Gpu);

void Sin_(struct GPU* Gpu);

void Push_(struct GPU* Gpu);

void RegPush_(struct GPU* Gpu);

void RegPop_(struct GPU* Gpu);

void Calculate(struct GPU* Gpu);

void GpuActivation(struct GPU* Gpu);

void GpuUpdate(struct GPU* Gpu, char* file_in, FILE* fp_in);

void GpuShutdown(struct GPU* Gpu);

void GpuDump(struct GPU* Gpu, const char* file_name, FILE* fp, GpuErrors err_no);

GpuErrors GpuOk(struct GPU* Gpu);
