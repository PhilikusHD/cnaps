#pragma once
#include <random>
#include <vector>
#include "shared/Types.h"

namespace RandomUtils
{
	// Seed with a random device
	static std::random_device rd;
	static std::mt19937 rng(rd());

	// Generate a random integer in a range
	static int RandomInt(size_t min, size_t max)
	{
		std::uniform_int_distribution<size_t> dist(min, max);
		return dist(rng);
	}

	// Shuffle a container
	template<typename T>
	static void Shuffle(std::vector<T>& container)
	{
		std::shuffle(container.begin(), container.end(), rng);
	}

	// Pick a random element from a container
	template<typename T>
	static T RandomElement(const std::vector<T>& container)
	{
		if (container.empty()) throw std::runtime_error("Container is empty");
		int index = RandomInt(0, container.size() - 1);
		return container[index];
	}
}
