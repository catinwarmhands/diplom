#pragma once
#include "utils.cpp"

#include "Vector.cpp"
#include "Matrix.cpp"

template <class T>
struct Image {
	T *data;
	int height, width, depth;
	Matrix<T> *channels;
};

template <class T>
void destroy(Image<T> image) {
	stbi_image_free(image.data);
	for (int i = 0; i < image.depth; ++i) {
		delete[] image.channels[i].rows;
		delete[] image.channels[i].columns;
	}
	delete[] image.channels;
}

template <class T>
Image<T> create_Image(int height, int width, int depth) {
	T *data = (T*)STBI_MALLOC(height*width*depth*sizeof(T));
	Matrix<T> *channels = new Matrix<T>[depth];

	for (int i = 0; i < depth; ++i)
		channels[i] = place_Matrix(data+i, height, width, depth);

	return Image<T>{data, height, width, depth, channels};
}

Image<byte> load_image(string filename) {
	int width, height, depth;
	byte *data = stbi_load(filename.c_str(), &width, &height, &depth, 0);

	if (!data) {
		std::cout << "load_image(" << filename << ") fail\n";
		std::exit(0);
	}

	Matrix<byte> *channels = new Matrix<byte>[depth];

	for (int k = 0; k < depth; ++k)
		channels[k] = place_Matrix(data+k, height, width, depth);

	return Image<byte>{data, height, width, depth, channels};
}

void write_image(Image<byte> image, string filename) {
	auto type  = filename.substr(filename.length()-3, 3);
	auto w     = image.width;
	auto h     = image.height;
	auto depth = image.depth;
	auto *data = image.data;

	if (type == "png") 
		stbi_write_png(filename.c_str(), w, h, depth, data, w*depth);
	else if (type == "bmp")
		stbi_write_bmp(filename.c_str(), w, h, depth, data);
	else if (type == "tga")
		stbi_write_tga(filename.c_str(), w, h, depth, data);
	else
		std::cout << "write_image(" << filename << ") fail: wrong file extension\n";
}

template<class T>
Image<T> copy(Image<T> image) {
	auto result = create_Image<T>(image.height, image.width, image.depth);
	auto size = image.height * image.width * image.depth;
	for (int i = 0; i < size; ++i)
		result.data[i] = image.data[i];
	return result;
}

Image<Complex> to_Complex(Image<byte> image) {
	auto result = create_Image<Complex>(image.height, image.width, image.depth);
	auto size = image.height * image.width * image.depth;

	for (int i = 0; i < size; ++i)
		result.data[i] = image.data[i];

	//shift (multiply to (-1)^(i+j))
	for (int i = 0; i < result.height; ++i)
		for (int j = 1-i%2; j < result.width; j+=2)
			for (int k = 0; k < result.depth; ++k)
				result.channels[k].rows[i][j] *= -1;

	return result;
}

Image<byte> to_byte(Image<Complex> image) {
	auto result = create_Image<byte>(image.height, image.width, image.depth);
	auto size   = image.height * image.width * image.depth;

	for (int i = 0; i < size; ++i) {
		auto temp = round(abs(image.data[i]));
		result.data[i] = (temp < 255) ? temp : 255;
	}
	return result;
}

template <class T>
std::ostream& operator<<(std::ostream& os, Image<T> image) {
	return os << "Image{width = " << image.width << ", height = " << image.height << ", depth = " << image.depth << "}";
}

template<typename T>
bool operator==(Image<T> lhs, Image<T> rhs) { return to_Vector(lhs) == to_Vector(rhs); }

template<typename T>
bool operator!=(Image<T> lhs, Image<T> rhs) { return !(lhs == rhs); }