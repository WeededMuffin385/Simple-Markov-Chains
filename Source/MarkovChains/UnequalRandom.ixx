#include <unordered_map>
#include <cstdint>
#include <random>

#include <iostream>

export module Sandcore.UnequalRandom;

export namespace Sandcore {
	std::mt19937 random = std::mt19937(std::random_device()());

	template <typename T>
	const T& getRandomValue(std::unordered_map<T, std::uint64_t>& chances, std::uint64_t max) {
		std::uniform_int_distribution<std::uint64_t> dist(1, max);
		std::uint64_t chance = dist(random);
		std::uint64_t border = 1;

		for (auto& [key, value] : chances) {
			if (chance >= border) {
				if (chance < (border + value)) {
					return key;
				}
			}
			border += value;
		}
	}
}