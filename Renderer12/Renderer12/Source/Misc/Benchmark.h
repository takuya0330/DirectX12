#pragma once
#include <chrono>

class Benchmark
{
	// https://qiita.com/suin/items/1ecba7204207568a51e3
private:
	std::chrono::system_clock::time_point mBegin;
	std::chrono::system_clock::time_point mEnd;
	double mMilliSecond = 0;
public:
	void Begin()
	{
		mBegin = std::chrono::system_clock::now();
	}
	void End()
	{
		mEnd = std::chrono::system_clock::now();
		mMilliSecond = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(mEnd - mBegin).count() / 1000.0);
	}
	double GetMilliSecond()noexcept
	{
		return mMilliSecond;
	}
public:
	Benchmark() = default;
	~Benchmark() = default;
};