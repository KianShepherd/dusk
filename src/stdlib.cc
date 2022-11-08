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

    void* newstruct(long size) {
        return calloc(1, (size_t)size);
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

    void delet(void* vec) {
        delete (std::vector<int>*)vec;
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

    void* new_str() {
        return new std::string();
    }

    void* new_str_preset(char* text) {
        return new std::string(text);
    }

    char* print_str(void* s) {
        return (char*)(void*)(((std::string*)s)->c_str());
    }
    // To strings
    char* itos(long a) {
        std::ostringstream oss;
        oss << a;
        std::string* s = new std::string(oss.str());
        const char* b = s->c_str();
        return copycs(b, s->size() + 1);
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


    void append_str_c(void* str, char* text) {
        ((std::string*)str)->append(text);
    }

    void append_str_s(void* str, void* text) {
        ((std::string*)str)->append((*((std::string*)text)));
    }

    char* to_string(void* str) {
        return (char*)(void*)(((std::string*)str)->c_str());
    }
}
