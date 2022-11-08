#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

char* copycs(const char* arr, long n) {
    char* heap_arr = (char*)malloc(sizeof(char) * n);
    memcpy(heap_arr, arr, n * sizeof(char));
    return heap_arr;
}


extern "C" DLLEXPORT {
    char* input() {
        std::string strtmp;
        std::getline(std::cin, strtmp);
        char* tmp = copycs(strtmp.c_str(), (int)strtmp.size());
        return tmp;
    }
    
    char* readfile(char* filename) {
        std::ifstream t(filename);
        std::stringstream buffer;
        buffer << t.rdbuf();
        std::string content = buffer.str();
        return copycs(content.c_str(), content.size());
    }

    // print
    long print(char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        return 0;
    }

    long println(char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
        return 0;
    }
}
