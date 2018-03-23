#pragma once

#include <chrono>

class Timer {
	using Time = std::chrono::high_resolution_clock;
	using ms = std::chrono::milliseconds;
	
  private:
	Time::time_point m_Start;
	Time::time_point m_Current;
  public:
	Timer() {
		m_Current = m_Start = Time::now();
	}
	auto leap() {
		auto now  = Time::now();
		auto old  = now-m_Current;
		m_Current = now;
		return std::chrono::duration_cast<ms>(old).count();
	}
};
