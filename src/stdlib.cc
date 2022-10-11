#include <stdio.h>
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT long printd(double X) {
  printf("%f\n", X);
  return 0;
}

extern "C" DLLEXPORT long printi(long X) {
  printf("%ld\n", X);
  return 0;
}
