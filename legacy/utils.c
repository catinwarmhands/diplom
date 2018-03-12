#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

typedef  int8_t  s8;
typedef  int16_t s16;
typedef  int32_t s32;
typedef  int64_t s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 byte;

typedef float  float32;
typedef double float64;
typedef float  f32;
typedef double f64;

#include "Array.c"

bool ends_with(const char *str, const char *suffix)
{
	if (!str || !suffix) {
		return 0;
	}
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if (lensuffix > lenstr) {
		return 0;
	}
	return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}