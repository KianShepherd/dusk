#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h> 
#include <cstring>
#include <sstream>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT {
    long* copyi(long* arr, long n) {
        long* heap_arr = (long*)malloc(sizeof(long) * n);
        memcpy(heap_arr, arr, n * sizeof(long));
        return heap_arr;
    }

    double* copyd(double* arr, long n) {
        double* heap_arr = (double*)malloc(sizeof(double) * n);
        memcpy(heap_arr, arr, n * sizeof(double));
        return heap_arr;
    }

    char* copys(char* arr, long n) {
        char* heap_arr = (char*)malloc(sizeof(char) * n);
        memcpy(heap_arr, arr, n * sizeof(char));
        return heap_arr;
    }

    char* copycs(const char* arr, long n) {
        char* heap_arr = (char*)malloc(sizeof(char) * n);
        memcpy(heap_arr, arr, n * sizeof(char));
        return heap_arr;
    }

    char** copysa(char** arr, long n) {
        char** heap_arr = (char**)malloc(n * sizeof(char*)) ;
        for (int i = 0; i < n; i++) {
            heap_arr[i] = strdup(arr[i]);
        }
        return heap_arr;
    }

    long del(void* arr) {
        free((void*)arr);
        return 1;
    }

    // To longs
    long long ftol(double a) {
        return static_cast<long long>(a);
    }

    long long itol(long a) {
        return static_cast<long long>(a);
    }

    long long stol(char* a) {
        return std::atoll(a);
    }
    // To ints
    long ftoi(double a) {
        return static_cast<long>(a);
    }

    long ltoi(long long a) {
        return static_cast<long>(a);
    }

    long stoi(char* a) {
        return std::atol(a);
    }
    // To floats
    double itof(long a) {
        return static_cast<double>(a);
    }

    double ltof(long long a) {
        return static_cast<double>(a);
    }

    double stof(char* a) {
        return std::atof(a);
    }
    // To strings
    char* itos(long a) {
        std::ostringstream oss;
        oss << a;
        std::string s = oss.str();
        const char* b = s.c_str();
        return copycs(b, s.size() + 1);
    }

    char* ltos(long long a) {
        std::ostringstream oss;
        oss << a;
        std::string s = oss.str();
        const char* b = s.c_str();
        return copycs(b, s.size() + 1);
    }

    char* ftos(double a) {
        std::ostringstream oss;
        oss << a;
        std::string s = oss.str();
        const char* b = s.c_str();
        return copycs(b, s.size() + 1);
    }

    // print
    long print(char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        return 0;
    }
}
