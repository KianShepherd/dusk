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
#include <cmath>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif


extern "C" DLLEXPORT {
    long ipow(long base, long power) {
        long b = base;
        for (int i = 0; i < power - 1; i++) {
            b *= base;
        }
        return b;
    }

    double fpow(double base, double power) {
        float b = static_cast<float>(base);
        float p = static_cast<float>(power);
        return (double)pow(b, p);
    }
}
