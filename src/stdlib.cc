#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h> 
#include <cstring>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif


extern "C" DLLEXPORT long* copyi(long* arr, long n) {
    long* heap_arr = (long*)malloc(sizeof(long) * n);
    memcpy(heap_arr, arr, n * sizeof(long));
    return heap_arr;
}

extern "C" DLLEXPORT double* copyd(double* arr, long n) {
    double* heap_arr = (double*)malloc(sizeof(double) * n);
    memcpy(heap_arr, arr, n * sizeof(double));
    return heap_arr;
}

extern "C" DLLEXPORT long del(void* arr) {
    free((void*)arr);
    return 1;
}

extern "C" DLLEXPORT long print(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
  return 0;
}
