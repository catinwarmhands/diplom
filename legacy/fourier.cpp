#pragma once
#include "utils.cpp"

#include "math.cpp"
#include "Image.cpp"

//////////////////////////////////////////////
/////////////// FOURIER //////////////////////
//////////////////////////////////////////////


//for integers only
template <class T>
bool is_power_of_2(T n) {
	return (n & (n - 1)) == 0;
}

template <class T>
T reverse_bits(T original, int size = sizeof(T)*8) {
	T reversed = 0;
	auto a = size-1;
	for(int i = 0 ; i < size; i++)
		reversed |= ((original>>i) & 1)<<(a-i);
	return reversed;
}

Vector<Complex> dft(Vector<Complex> vec) {
	int   N  = vec.size;
	float dt = 1.0/sqrt(N);

	Vector<Complex> result = create_Vector<Complex>(N);

	for (int u = 0; u <= N-1; ++u)
		for (int x = 0; x <= N-1; ++x)
			result[u] += dt * vec[x] * exp(Complex(0, -pi2*u*x/N));

	return result;
}

Vector<Complex> fft(Vector<Complex> vec) {
	int   N  = vec.size;
	int   n  = log2(N);
	float dt = 1.0/sqrt(N);
	
	Vector<Complex> result = create_Vector_uninitialized<Complex>(vec.size);

	for (int i = 0; i < N; ++i)
		result[i] = vec[reverse_bits(i, n)];

	for (int i = 1; i <= n; ++i) { //шаг
		int blockSize = pow(2, i);
		int halfBlockSize = blockSize/2;
		int blocksAmount = N/blockSize;
		for (int j = 0; j < blocksAmount; ++j) { //номер блока
			int cur = j*blockSize; //начало обрабатываемого блока

			//домножаем вторую половину блока на w_i^k
			//пропускаем k = 0 за ненадобностью
			for (int l = halfBlockSize+1, k = 1; l < blockSize; ++l, ++k) {
				result[cur+l] *= exp(Complex(0, -pi2/pow(2, i)*k));
			}

			//перекрестное сложение вычитание
			for (int k = 0; k < halfBlockSize; ++k) {
				auto& a = result[cur+k];
				auto& b = result[cur+k+halfBlockSize];
				auto tempA = a;
				a = tempA+b;
				b = tempA-b;
			}
		}
	}

	for (int i = 0; i < N; ++i)
		result[i] *= dt;

	return result;
}

Matrix<Complex> fourier(Matrix<Complex> mat) {
	auto N = mat.height;
	auto M = mat.width;

	Matrix<Complex> result = create_Matrix<Complex>(N, M);

	PARALLEL_FOR(i, 0, i < N, ++i, {
		//use fft if possible
		auto temp = is_power_of_2(mat.width) ? fft(mat.rows[i]) : dft(mat.rows[i]);
		defer(destroy(temp));

		for (int j = 0; j < M; ++j)
			result.rows[i][j] = temp[j];
	});

	PARALLEL_FOR(j, 0, j < M, ++j, {
		auto temp = is_power_of_2(mat.height) ? fft(result.columns[j]) : dft(result.columns[j]);
		defer(destroy(temp));

		for (int i = 0; i < N; ++i)
			result.columns[j][i] = temp[i];
	});

	return result;
}

Image<Complex> fourier(Image<Complex> image) {
	auto result = create_Image<Complex>(image.height, image.width, image.depth);
	auto size = result.height * result.width;

	for (int k = 0; k < result.depth; ++k) {
		auto temp = fourier(image.channels[k]);
		defer(destroy(temp));

		for (int i = 0; i < size; ++i)
			result.channels[k][i] = temp[i];
	}

	return result;
}

//////////////////////////////////////////////
/////////////// INV_FOURIER //////////////////
//////////////////////////////////////////////


Vector<Complex> inv_dft(Vector<Complex> vec) {
	int    N  = vec.size;
	float dt = 1.0/sqrt(N);
	float dv = 1.0/(N*dt);

	Vector<Complex> result = create_Vector<Complex>(N);

	for (int x = 0; x <= N-1; ++x)
		for (int u = 0; u <= N-1; ++u)
			result[x] += dv* vec[u] * exp(Complex(0, pi2*u*x/N));
	
	return result;
}

Vector<Complex> inv_fft(Vector<Complex> vec) {
	int     N  = vec.size;
	int     n  = log2(N);
	float dt = 1.0/sqrt(N);
	float dv = 1.0/(N*dt);
	
	Vector<Complex> result = create_Vector_uninitialized<Complex>(vec.size);

	for (int i = 0; i < N; ++i) {
		result[i] = vec[reverse_bits(i, n)];
	}

	for (int i = 1; i <= n; ++i) { //шаг
		int blockSize = pow(2, i);
		int halfBlockSize = blockSize/2;
		int blocksAmount = N/blockSize;
		for (int j = 0; j < blocksAmount; ++j) { //номер блока
			int cur = j*blockSize; //начало обрабатываемого блока

			//домножаем вторую половину блока на w_i^k
			//пропускаем k = 0 за ненадобностью
			for (int l = halfBlockSize+1, k = 1; l < blockSize; ++l, ++k) {
				result[cur+l] *= exp(Complex(0, pi2/pow(2, i)*k));
			}

			//перекрестное сложение вычитание
			for (int k = 0; k < halfBlockSize; ++k) {
				auto& a = result[cur+k];
				auto& b = result[cur+k+halfBlockSize];
				auto tempA = a;
				a = tempA+b;
				b = tempA-b;
			}
		}
	}

	for (int i = 0; i < N; ++i) {
		result[i] *= dv;
	}

	return result;
}

Matrix<Complex> inv_fourier(Matrix<Complex> mat) {
	int N = mat.height;
	int M = mat.width;

	Matrix<Complex> result = create_Matrix<Complex>(N, M);

	PARALLEL_FOR(j, 0, j < M, ++j, {
		auto temp = is_power_of_2(mat.height) ? inv_fft(mat.columns[j]) : inv_dft(mat.columns[j]);
		defer(destroy(temp));

		for (int i = 0; i < N; ++i)
			result.columns[j][i] = temp[i];
	});

	PARALLEL_FOR(i, 0, i < N, ++i, {
		auto temp = is_power_of_2(mat.width) ? inv_fft(result.rows[i]) : inv_dft(result.rows[i]);
		defer(destroy(temp));

		for (int j = 0; j < M; ++j)
			result.rows[i][j] = temp[j];
	});

	return result;
}

Image<Complex> inv_fourier(Image<Complex> image) {
	auto result = create_Image<Complex>(image.height, image.width, image.depth);
	auto size = result.height * result.width;

	for (int k = 0; k < result.depth; ++k) {
		auto temp = inv_fourier(image.channels[k]);
		defer(destroy(temp));

		for (int i = 0; i < size; ++i)
			result.channels[k][i] = temp[i];
	}

	return result;
}