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

#ifdef GPU_DEBUG_BARANOV

#define ASSERT_GPU(Gpu) {                        \
    GpuErrors err_no = GpuOk(Gpu);               \
    if (err_no != GPU_OK) {                      \
        GPU_DUMP(Gpu, err_no);                   \
        assert(!"OK");                           \
    }                                            \
}
#else

#define ASSERT_GPU(Gpu)

#endif


//static const int MAX_NAME_LEN = 100;

//static char file_in[MAX_NAME_LEN];

#define DEF_CMD(upper_name, num, reg, code) \
    ExitCodes upper_name##_(struct GPU* Gpu);

#include "Code_Gen_Gpu.h"

#undef DEF_CMD

type_t PopRam(struct GPU* Gpu, int index);

void PushRam(struct GPU* Gpu, int index, type_t value);

void Calculate(struct GPU* Gpu);

void GpuActivation(struct GPU* Gpu);

void GpuUpdate(struct GPU* Gpu, char* file_in, FILE** fp_in);

void GpuShutdown(struct GPU* Gpu);

void GpuDump(struct GPU* Gpu, const char* file_name, FILE* fp, GpuErrors err_no);

GpuErrors GpuOk(struct GPU* Gpu);

void PrintRam(struct GPU* Gpu);
