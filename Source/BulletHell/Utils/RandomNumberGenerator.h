#pragma once

#include <random>

class RandomNumberGenerator
{
public:
	static int GenerateInteger(int min, int max) {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> uni(min, max);

		return uni(rng);
	}

	static float GenerateFloat(float min, float max) {
		std::random_device rd;
		std::mt19937 rng(rd());
		std::uniform_real_distribution<float> uni(min, max);

		return uni(rng);
	}
};