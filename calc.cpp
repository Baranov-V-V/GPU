//#include "TXLib.h"
#include "Calc.h"
#include "Calc_functions.cpp"
#include "Gpu_functuons.cpp"

int main() {
    setlocale(LC_ALL, "Russian");

    struct GPU Gpu = {};
    GpuActivation(&Gpu);

    char* file_in = (char*) calloc(100, 1);
    printf("file in: ");
    scanf("%s", file_in);
    FILE* fp_in = NULL;
    //printf("done");
    GpuUpdate(&Gpu, file_in, &fp_in);
    //printf("done2");
    Calculate(&Gpu);
    exit(0);
    //printf("done3");

    return 0;
}

