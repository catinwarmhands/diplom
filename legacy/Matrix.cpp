#pragma once
#include "utils.cpp"

#include "Vector.cpp"

template<class T>
struct Matrix {
	T *data;
	int height, width;
	int stride = 1;
	Vector<T> *rows    = nullptr;
	Vector<T> *columns = nullptr;

	T& operator[](int i) { return data[stride*i]; }
};

template<class T>
Matrix<T> place_Matrix(T *data, int height, int width, int stride) {
	auto mat = Matrix<T>{data, height, width, stride};
	mat.rows    = new Vector<T>[height];
	mat.columns = new Vector<T>[width];

	for (int i = 0; i < height; ++i) mat.rows[i]    = Vector<T>{data+i*width*stride,  width,       stride};
	for (int i = 0; i < width;  ++i) mat.columns[i] = Vector<T>{data+i*stride      , height, width*stride};

	return mat;
}

template<class T>
Matrix<T> create_Matrix_uninitialized(int height, int width) {
	T *data = new T[height * width];
	auto mat = place_Matrix(data, height, width, 1);
	return mat;
}

template<class T>
Matrix<T> create_Matrix(int height, int width) {
	auto mat = create_Matrix_uninitialized<T>(height, width);
	auto size = height*width;
	for (int i = 0; i < size; ++i)
		mat[i] = 0;
	return mat;
}

template<class T>
void destroy(Matrix<T> mat) {
	delete[] mat.data;
	delete[] mat.rows;
	delete[] mat.columns;
}

template<class T>
Matrix<T> copy(Matrix<T> mat) {
	auto result = create_Matrix_uninitialized<T>(mat.height, mat.width);
	auto size = mat.height*mat.width;
	for (int i = 0; i < size; ++i)
		result[i] = mat[i];
	return result;
};

template<class T>
Matrix<Complex> to_Complex(Matrix<T> mat) {
	auto N = mat.height;
	auto M = mat.width;
	auto NM = N*M;
	auto result = create_Matrix_uninitialized<Complex>(N, M);
	int k = 0;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
			result.data[k++] = mat.rows[i][j];
	return result;
}

Matrix<byte> to_byte(Matrix<Complex> mat) {
	auto N = mat.height;
	auto M = mat.width;
	auto NM = N*M;
	auto result = create_Matrix_uninitialized<byte>(N, M);
	for (int i = 0; i < NM; ++i) {
		auto temp = round(abs(mat.data[i]));
		result.data[i] = (temp > 255) ? 255 : temp;
	}
	return result;
}

template<typename T>
bool operator==(Matrix<T> lhs, Matrix<T> rhs) {
	if ((lhs.width != rhs.width) || (lhs.height != rhs.height)) return false;
	for (int i = 0; i < lhs.height; ++i)
		if (lhs.rows[i] != rhs.rows[i]) return false;
	return true;
}

template<typename T>
bool operator!=(Matrix<T> lhs, Matrix<T> rhs) { return !(lhs == rhs); }

template<typename T>
std::ostream& operator<<(std::ostream& os, Matrix<T> mat) {
	os << '{';
	for (int i = 0; i < mat.height; ++i) {
		os << mat.rows[i];
		if (i != mat.height-1) os << ", ";
	}
	return os << '}';
}