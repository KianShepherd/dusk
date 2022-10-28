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

    long* ialloc(long n) {
        return (long*)malloc(n * sizeof(long));
    }

    double* falloc(long n) {
        return (double*)malloc(n * sizeof(double));
    }

    long del(void* arr) {
        free((void*)arr);
        return 1;
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

    long count_substr(char* str, char* sub) {
        std::string s = std::string(str);
        std::string::difference_type n = std::count(s.begin(), s.end(), (char)sub[0]);
        return (long)n;
    }

    char** split(char* str, char* split_on) {
        std::stringstream test(str);
        std::string segment;
        std::vector<std::string> seglist;
        while(std::getline(test, segment, (char)split_on[0]))
        {
            seglist.push_back(segment);
        }
        auto len = seglist.size();
        char** out = (char**)malloc(len * sizeof(char*));
        for (int i = 0; i < len; i++) {
            out[i] = copycs(seglist[i].c_str(), seglist[i].size());
        }
        return out;
    }

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
    
    void* intvec() {
        return new std::vector<long>();
    }

    void ipush_back(void* vec, long n) {
        (*(std::vector<long>*)vec).push_back(n);
    }

    long getveci(void* vec, long idx) {
        return ((*(std::vector<long>*)vec)[idx]);
    }

    long sizeveci(void* vec) {
        return (long)(*(std::vector<long*>*)vec).size();
    }

    void* floatvec() {
        return new std::vector<double>();
    }

    void fpush_back(void* vec, double n) {
        (*(std::vector<double>*)vec).push_back(n);
    }

    double getvecf(void* vec, long idx) {
        return (*(std::vector<double>*)vec)[idx];
    }

    long sizevecf(void* vec) {
        return (long)(*(std::vector<double>*)vec).size();
    }
}
