#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils.c"
#include "Array.c"
#include "Image.c"

#include "gl.c"

// DefineArray(byte);
DefineArray(double);

void my_printing_function(s64 *n) {
	printf("MYINT(%lld)", *n);
}

void print_byte_as_int(byte* n) {
	printf("%d", *n);
}

int main() {
	int *a = malloc(10*sizeof(int));
	for (int i = 0; i < 10; ++i) {
		a[i] = i;
	}
	for (int i = 0; i < 10; ++i) {
		printf("%d ", a[i]);
	}
	game_create_window(200,200);
	// double complex dc1 = 3 + 2*I;
 //    double complex dc2 = 4 + 5*I;
 //    double complex result;

 //    result = dc1 + dc2;
 //    printf(" ??? \n", result);
	// Array(byte) a1 = array_create(byte)(0);
	// Array(double) a2 = array_create(double)(0);

	// for (int i = 0; i < 15; ++i) {
	// 	array_add(byte)(&a1, rand());
	// 	array_add(double)(&a2, rand()*1.1);
	// }

	// array_print(byte)(&a1, print_byte_as_int);
	// array_print(double)(&a2, NULL);

	// for (int i = 0; i < 15; ++i) {
	// 	array_add(s64)(&array, rand()%10);
	// }

	// array_print(s64)(&array, NULL);
	// array_print(s64)(&array, my_printing_function);
	
	// Image image = image_load("img/test1.png");
	// array_print(byte)(&image.data, NULL);
	// image_write(&image, "test.png");
}