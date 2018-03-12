#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#include "libs/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb/stb_image_write.h"

#include "utils.c"

DefineArray(byte);

typedef struct Image {
	Array_byte data;
	int height, width, depth;
} Image;

Image image_load(const char *filename) {
	Image image;
	byte *data = stbi_load(filename, &image.width, &image.height, &image.depth, 0);
	image.data = array_create_from_memory(byte)(data, image.width * image.height * image.depth);

	if (!image.data.data) {
		printf("image_load(%s) fail\n", filename);
		abort();
	}
	return image;
	// Matrix<byte> *channels = new Matrix<byte>[depth];
	// for (int k = 0; k < depth; ++k)
	// 	channels[k] = place_Matrix(data+k, height, width, depth);
	// return Image<byte>{data, height, width, depth, channels};
}


bool image_write(const Image *image, const char *filename) {
	byte *data = image->data.data;
	int w      = image->width;
	int h      = image->height;
	int depth  = image->depth;

	if (ends_with(filename, ".png")) {
		stbi_write_png(filename, w, h, depth, data, w*depth);
	}
	else if (ends_with(filename, ".bmp")) {
		stbi_write_bmp(filename, w, h, depth, data);
	}
	else if (ends_with(filename, ".tga")) {
		stbi_write_tga(filename, w, h, depth, data);
	}
	else {
		printf("image_write(%s) fail: wrong extension\n", filename);
		return false;
	}
	return true;
}