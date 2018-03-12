#pragma once

#include <thread>

#define MAX_THREADS 8

struct Threads {
	std::thread threads[MAX_THREADS];
	int size = 0;

	template<class Func, class ... Args>
	void add(Func&& f, const Args&... a) {
		if (size >= MAX_THREADS-1)
			join();
		threads[size++] = std::thread(f, a...);
	}

	void join() {
		for (int i = 0; i < size; ++i)
			threads[i].join();
		size = 0;
	}
};

#define PARALLEL_FOR(i, start, cond, post, func) \
{                                                \
	Threads _threads;                            \
	for (auto i = start; cond; post) {           \
		_threads.add([&](auto i){func}, i);      \
	}                                            \
	_threads.join();                             \
}
