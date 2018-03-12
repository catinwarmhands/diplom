#pragma once

#include <cmath>

#define T float
#define USE_STD_COMPLEX false

#if USE_STD_COMPLEX
	#include <complex>
	using Complex = std::complex<T>;
#else

struct Complex {
	T re, im;

	Complex(T a = 0, T b = 0) {
		re = a;
		im = b;
	}
};

bool operator==(const Complex &a, const Complex &b) { return (a.re == b.re) && (a.im == b.im); }
bool operator!=(const Complex &a, const Complex &b) { return (a.re != b.re) || (a.im != b.im); }

Complex operator+(const Complex &a) { return a; }
Complex operator-(const Complex &a) { return Complex{-a.re, -a.im}; }
Complex operator+(const Complex &a, const Complex &b) { return Complex(a.re + b.re, a.im + b.im); }
Complex operator-(const Complex &a, const Complex &b) { return Complex(a.re - b.re, a.im - b.im); }
Complex operator*(const Complex &a, const Complex &b) { return Complex(a.re*b.re - a.im*b.im, a.im*b.re + a.re*b.im); }
Complex operator/(const Complex &a, const Complex &b) {
	auto c = b.re*b.re +  b.im*b.im;
	return Complex{(a.re*b.re + a.im*b.im)/c, (a.im*b.re - a.re*b.im)/c};
}

template<class K> Complex operator+(const Complex &a, K b) { return Complex(a.re + b, a.im); }
template<class K> Complex operator-(const Complex &a, K b) { return Complex(a.re - b, a.im); }
template<class K> Complex operator*(const Complex &a, K b) { return Complex(a.re * b, a.im * b); }
template<class K> Complex operator/(const Complex &a, K b) { return Complex(a.re / b, a.im / b); }

template<class K> Complex operator+(K a, const Complex &b) { return Complex(a + b.re, b.im); }
template<class K> Complex operator-(K a, const Complex &b) { return Complex(a - b.re, b.im); }
template<class K> Complex operator*(K a, const Complex &b) { return Complex(a * b.re, a * b.im); }
template<class K> Complex operator/(K a, const Complex &b) { return Complex(a / b.re, a / b.im); }

template<class K> Complex &operator+=(Complex &a, const K &b) { return a = a + b; }
template<class K> Complex &operator-=(Complex &a, const K &b) { return a = a - b; }
template<class K> Complex &operator*=(Complex &a, const K &b) { return a = a * b; }
template<class K> Complex &operator/=(Complex &a, const K &b) { return a = a / b; }

#include <iostream>
std::ostream &operator<<(std::ostream &os, Complex z) {
	return os << "(" << z.re << (z.im < 0 ? "" : "+") << z.im << "i)";
}

T real(const Complex &z) { return z.re; }
T imag(const Complex &z) { return z.im; }
T norm(const Complex &z) { return z.re*z.re + z.im*z.im; }
T abs(const Complex &z)  { return sqrt(norm(z)); }
T arg(const Complex &z)  { return atan2(z.im, z.re); }
Complex conj(const Complex &z) { return Complex{z.im, z.re}; }

Complex exp(const Complex &z) { 
	auto a = exp(z.re); 
	return Complex(a*cos(z.im), a*sin(z.im));
	// auto a = arg(z);
	// return Complex{cos(a), sin(a)};
}

Complex pow(const Complex &z, float n) { 
	auto a = arg(z);
	return std::pow(abs(z), n) * Complex(cos(n*a), sin(n*a));
}

Complex sin(const Complex &z) { return (exp(z)-exp(-z))/Complex{0, 2.0}; }
Complex cos(const Complex &z) { return (exp(z)+exp(-z))/2.0; }
Complex tan(const Complex &z) { 
	auto exp1 = exp(z);
	auto exp2 = exp(-z);
	return (exp1-exp2)/(exp1+exp2) * Complex{0, -1};
}
Complex cot(const Complex &z) { 
	auto exp1 = exp(z);
	auto exp2 = exp(-z);
	return (exp1+exp2)/(exp1-exp2) * Complex{0, 1};
}
Complex ch(const Complex &z) { return (exp(z)+exp(-z))/2.0; }
Complex sh(const Complex &z) { return (exp(z)-exp(-z))/2.0; }
Complex sec(const Complex &z) { return 2.0/(exp(z)+exp(-z)); }
Complex cosec(const Complex &z) { return 2.0/(exp(z)-exp(-z)); }

#undef T

#endif //else USE_STD_COMPLEX