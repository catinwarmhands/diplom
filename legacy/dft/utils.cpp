#pragma once

#include <iostream>
#include <ctime>
#include <string>
using std::string;
using std::cin;
using std::cout;

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb/stb_image_write.h"

#include "libs/defer.cpp"
#include "libs/timer.cpp"
#include "libs/Complex.cpp"
#include "libs/parallel_for.cpp"
#include "libs/create_dir.cpp"

#define print(x) std::cout << #x << " = " << (x) << "\n"
#define sqr(x) pow(x, 2)

using byte = unsigned char;

template<class T>
T min(T a, T b) {
	return a < b ? a : b;
}

string add_postfix(string filename, const string& postfix) {
	auto k = filename.rfind(".");
	if (k == string::npos)
		k = filename.length()-1;
	filename.insert(k, postfix);
	return filename;
};
