#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define GOLDEN_RATIO 1.61803398875
#define ARRAY_RESERVE_MULTIPLIER GOLDEN_RATIO

#define T float

#define Array(T)                           Array_##T
#define array_reserve(T)                   array_##T##_reserve
#define array_create_uninitialized(T)      array_##T##_create_uninitialized
#define array_create(T)                    array_##T##_create
#define array_create_from_memory(T)        array_##T##_create_from_memory
#define array_free(T)                      array_##T##_free
#define array_add(T)                       array_##T##_add
#define array_find(T)                      array_##T##_find
#define array_add_if_unique(T)             array_##T##_add_if_unique
#define array_print(T)                     array_##T##_print
#define array_unordered_remove_by_index(T) array_##T##_unordered_remove_by_index
#define array_unordered_remove(T)          array_##T##_unordered_remove
#define array_ordered_remove_by_index(T)   array_##T##_ordered_remove_by_index
#define array_ordered_remove(T)            array_##T##_ordered_remove

#define DefineArray(T)
typedef struct {
	T *data;
	int count, allocated;
} Array(T);

void array_reserve(T)(Array(T) *array, const int reserve) {
	if (reserve <= array->allocated) {
		return;
	}
	int size = sizeof(T);
	int num_bytes = reserve * size;

	T *new_memory = realloc(array->data, num_bytes);
	assert(new_memory != NULL);

	array->data = new_memory;
	array->allocated = reserve;
}

Array(T) array_create_uninitialized(T)(const int size) {
	Array(T) array;
	memset(&array, 0, sizeof(Array(T)));
	array_reserve(T)(&array, size); 
	array.count = size;
	return array;
}

Array(T) array_create(T)(const int size) {
	Array(T) array = array_create_uninitialized(T)(size);
	memset(array.data, 0, sizeof(T) * array.allocated);
	return array;
}

Array(T) array_create_from_memory(T)(const T *source, const int size) {
	Array(T) array;
	array.data = (T*)source;
	array.count = array.allocated = size;
	return array;
}

void array_free(T)(Array(T) *array) {
	free(array->data);
	memset(&array, 0, sizeof(Array(T)));
}

void array_add(T)(Array(T) *array, const T item) {
	if (array->count >= array->allocated) {
		int reserve = ARRAY_RESERVE_MULTIPLIER * array->allocated;
		if (reserve < 8) {
			reserve = 8;
		}
		array_reserve(T)(array, reserve);
	}

	array->data[array->count] = item;
	array->count += 1;
}

int array_find(T)(const Array(T) *array, const T item) {
	int size = sizeof(T);
	for (int i = 0; i < array->count; ++i) {
		if (memcmp(array->data + i, &item, size) == 0) {
			return i;
		}
	}
	return -1;
}

int array_add_if_unique(T)(Array(T) *array, const T item) {
	int index = array_find(T)(array, item);
	if (index == -1) {
		return -1;
	}
	array_add(T)(array, item);
	return array->count-1;
}

void array_print(T)(const Array(T) *array, void(*print_func)(T*)) {
	printf("Array(%d", array->count);
	if (array->count != array->allocated) {
		printf("/%d", array->allocated);
	}
	printf("){");

	for (int i = 0; i < array->count; ++i) {
		if (print_func != NULL) {
			print_func(&array->data[i]);
		} else {
			char* str;
			// #define tostr(a) #a
			// // #define same(a,b) tostr(a) == tostr(b)
			#define same(a,b) #a == #b

			if (same(T, int)) {
				str = "%d";
			} else if (same(T, float)) {
				str = "%f";
			}
			printf(str, array->data[i]);
		}
		if (i != array->count-1) {
			printf(", ");
		}
	}
	printf("}\n");
}

void array_unordered_remove_by_index(T)(Array(T) *array, const int index) {
	assert(index >= 0);
	assert(index < array->count);

	if (index != array->count-1) {
		int size = sizeof(T);

		T *current = array->data + index;
		T *source  = array->data + (array->count - 1);
		memcpy(current, source, size);
	}

	array->count -= 1;
}

int array_unordered_remove(T)(Array(T) *array, const T item) {
	int removed = 0;
	int size = sizeof(T);

	for (int i = 0; i < array->count; ++i) {
		T *current = array->data + i;
		if (memcmp(current, &item, size) == 0) {
			array_unordered_remove_by_index(T)(array, i);
			removed += 1;
			i -= 1;
		}
	}
	return removed;
}

void array_ordered_remove_by_index(T)(Array(T) *array, const int index) {
	assert(index >= 0);
	assert(index < array->count);

	if (index != array->count-1) {
		int size = sizeof(T) * (array->count - index);
		T *current = array->data + index;
		T *source  = current + 1;
		memmove(current, source, size);
	}

	array->count -= 1;
}

int array_ordered_remove(T)(Array(T) *array, const T item) {
	int removed = 0;
	int size = sizeof(T);

	for (int i = 0; i < array->count; ++i) {
		T *current = array->data + i;
		if (memcmp(current, &item, size) == 0) {
			array_ordered_remove_by_index(T)(array, i);
			removed += 1;
			i -= 1;
		}
	}
	return removed;
}