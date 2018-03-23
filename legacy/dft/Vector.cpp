#pragma once
#include "utils.cpp"

template<class T>
struct Vector {
	T *data;
	int size;
	int stride = 1;
	
	T& operator[](int i) {return data[stride*i];}
};

template<class T>
Vector<T> create_Vector_uninitialized(int size) {
	return Vector<T>{new T[size], size};
}

template<class T>
Vector<T> create_Vector(int size) {
	auto vec = create_Vector_uninitialized<T>(size);
	for (int i = 0; i < vec.size; ++i)
		vec[i] = 0;
	return vec;
}

template<class T>
void destroy(Vector<T> vec) {
	delete[] vec.data;
}

template<class T>
Vector<T> copy(Vector<T> vec) {
	auto result = create_Vector_uninitialized<T>(vec.size);
	for (int i = 0; i < vec.size; ++i)
		result[i] = vec[i];
	return result;
};

template<typename T>
Vector<Complex> to_Complex(Vector<T> vec) {
	auto result = create_Vector_uninitialized<Complex>(vec.size);
	for (int i = 0; i < vec.size; ++i)
		result.data[i] = vec.data[i];
	return result;
}

Vector<byte> to_byte(Vector<Complex> vec) {
	auto result = create_Vector_uninitialized<byte>(vec.size);
	for (int i = 0; i < vec.size; ++i) {
		auto temp = round(abs(vec.data[i]));
		result.data[i] = (temp > 255) ? 255 : temp;
	}
	return result;
}

template<typename T>
bool operator==(Vector<T> lhs, Vector<T> rhs) {
	if (lhs.size != rhs.size) return false;
	for (int i = 0; i < lhs.size; ++i)
		if (lhs[i] != rhs[i]) return false;
	return true;
}

template<typename T>
bool operator!=(Vector<T> lhs, Vector<T> rhs) { return !(lhs == rhs); }

template<typename T>
std::ostream& operator<<(std::ostream& os, Vector<T> vec) {
	os << '{';
	for (int i = 0; i < vec.size; ++i) {
		os << vec[i];
		if (i != vec.size-1) os << ", ";
		if (i >= 10) {
			os << "...";
			break;
		}
	}
	return os << '}';
}

//fix for byte vectors - show numbers instead of chars
template<>
std::ostream& operator<<(std::ostream& os, Vector<byte> vec) {
	os << '{';
	for (int i = 0; i < vec.size; ++i) {
		os << (int)vec[i];
		if (i != vec.size-1) os << ", ";
		if (i > 10) {
			os << "...";
			break;
		}
	}
	return os << '}';
}
