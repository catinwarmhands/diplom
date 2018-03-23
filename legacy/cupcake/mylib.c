#include <stdio.h>
#include <inttypes.h>

typedef  int32_t s32;

// #ifdef __cplusplus
// #define DLLEXPORT extern "C" __declspec(dllexport)
// #else
#define DLLEXPORT __declspec(dllexport)
// #endif

DLLEXPORT void my_hello(void) {
	printf("hello from my_hello!\n");
}

DLLEXPORT s32 my_sum(s32 a, s32 b) {
	return a + b+1;
}