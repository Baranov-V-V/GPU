#include "Calc.h"
#include "Ñalc_functions.cpp"
#include "Gpu_functuons.cpp"

int main() {
    struct GPU Gpu;
    GpuActivation(&Gpu);

    printf("file in: ");
    scanf("%s", file_in);
    FILE* fp_in = NULL;

    GpuUpdate(&Gpu, file_in, fp_in);

    Calculate(&Gpu);

    fclose(fp_in);

    GpuShutdown(&Gpu);
 }
