#include <stdio.h>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT int printd(double X) {
  printf("%f\n", X);
  return 0;
}

/// printi - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT int printi(int X) {
  printf("%d\n", X);
  return 0;
}
