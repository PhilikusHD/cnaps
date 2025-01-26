#pragma once
#include <chrono>
#include <functional>
#include <iostream>

namespace Utils
{
	// Measure the execution time of a function
	void MeasureExecutionTime(const std::string& taskName, const std::function<void()>& func)
	{
		auto start = std::chrono::high_resolution_clock::now();
		func();
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << taskName << " took " << duration << "ms\n";
	}

	// Assert with a message
	void Assert(bool condition, const std::string& message)
	{
		if (!condition)
		{
			std::cerr << "Assertion failed: " << message << std::endl;
			std::terminate();
		}
	}
}
