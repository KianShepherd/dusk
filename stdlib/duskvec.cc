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

void* newstruct(long size) {
    return calloc(1, (size_t)size);
}

struct __InternString {
    void* str;
    int _rc;
};

extern "C" {
    long vec_split(char* str, char* split_on, void* vec) {
        std::stringstream test(str);
        std::string segment;
        while(std::getline(test, segment, (char)split_on[0]))
        {
            __InternString* string_struct = new __InternString();
            string_struct->str = (void*)new std::string(segment);
            string_struct->_rc = 1;
            (*(std::vector<void*>*)vec).push_back((void*)string_struct);
        }
        return (long)((std::vector<void*>*)vec)->size();
    }

    long vec_size(void* vec) {
        return (long)((std::vector<void*>*)vec)->size();
    }

    char* ivec_str(void* vec) {
        std::ostringstream str;
        str << "[ ";
        auto v = *((std::vector<long>*) vec);
        for (int i = 0; i < v.size(); i++) {
            str << v[i];
            if (i != v.size() - 1) {
                str << ", ";
            }
        }
        str << " ]";
        auto stri = new std::string(str.str());
        return (char*)(void*)stri->c_str();
    }

    char* fvec_str(void* vec) {
        std::ostringstream str;
        str << "[ ";
        auto v = *((std::vector<double>*) vec);
        for (int i = 0; i < v.size(); i++) {
            str << v[i];
            if (i != v.size() - 1) {
                str << ", ";
            }
        }
        str << " ]";
        auto stri = new std::string(str.str());
        return (char*)(void*)stri->c_str();
    }
    
    void* intvec() {
        return new std::vector<long>();
    }

    void del_ivec(void* v) {
        delete (std::vector<long>*)v;
    }

    void ipush_back(void* vec, long n) {
        (*(std::vector<long>*)vec).push_back(n);
    }

    void ipush_front(void* vec, long n) {
        ((std::vector<long>*)vec)->insert(((std::vector<long>*)vec)->begin(), n);
    }

    long getveci(void* vec, long idx) {
        return ((*(std::vector<long>*)vec)[idx]);
    }
    
    void sortveci(void* vec) {
        std::sort(((std::vector<long>*)vec)->begin(), ((std::vector<long>*)vec)->end());
    }
    
    void reverseveci(void* vec) {
        std::reverse(((std::vector<long>*)vec)->begin(), ((std::vector<long>*)vec)->end());
    }

    void* floatvec() {
        return new std::vector<double>();
    }

    void del_floatvec(void* v) {
        delete (std::vector<double>*)v;
    }

    void fpush_back(void* vec, double n) {
        (*(std::vector<double>*)vec).push_back(n);
    }

    void fpush_front(void* vec, double n) {
        ((std::vector<double>*)vec)->insert(((std::vector<double>*)vec)->begin(), n);
    }

    double getvecf(void* vec, long idx) {
        return (*(std::vector<double>*)vec)[idx];
    }

    void sortvecf(void* vec) {
        std::sort(((std::vector<double>*)vec)->begin(), ((std::vector<double>*)vec)->end());
    }

    void reversevecf(void* vec) {
        std::reverse(((std::vector<double>*)vec)->begin(), ((std::vector<double>*)vec)->end());
    }

    void* vpvec() {
        return new std::vector<void*>();
    }

    void del_vpvec(void* v) {
        delete (std::vector<void*>*)v;
    }

    void vppush_back(void* vec, void* n) {
        (*(std::vector<void*>*)vec).push_back(n);
    }

    void vppush_front(void* vec, void* n) {
        ((std::vector<void*>*)vec)->insert(((std::vector<void*>*)vec)->begin(), n);
    }

    void* getvecvp(void* vec, long idx) {
        return ((*(std::vector<void*>*)vec)[idx]);
    }
}
