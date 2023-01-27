#include <filesystem>
#include <iostream>
#include <fstream>
#include <random>

import Sandcore.MarkovChains.Enchanced;

namespace Sandcore {
	MarkovChainsEnchanced::MarkovChainsEnchanced() : dist(0, 1) {
		random.seed(std::random_device()());
	}

	void MarkovChainsEnchanced::uploadData(std::filesystem::path path) {
		isNormalized = false;
		std::ifstream file(path);

		while (!file.eof()) {
			std::string data;
			file >> data;
			if (data.empty()) continue;


			if (data.size() >= chainLength) ++firstChain[std::string(&data[0], &data[chainLength])];
			++length[data.size()];


			std::string prefix;
			for (auto c : data) {
				if (prefix.size() == chainLength) {
					++dictionary[prefix][c];
					prefix.erase(0, 1);
				}
				prefix += c;
			}
		}
	}

	void MarkovChainsEnchanced::normalizeChances() {
		if (isNormalized) return;
		isNormalized = true;
		normalizeDictionaryChances();
		normalizeFirstChainChances();
		normalizeLength();
	}

	void MarkovChainsEnchanced::normalizeDictionaryChances() {
		dictionaryNormalized = dictionary;

		for (auto& [key, value] : dictionaryNormalized) {
			double sum = 0;

			for (auto& [key2, value2] : value) {
				sum += value2;
			}

			for (auto& [key2, value2] : value) {
				value2 /= sum;
			}
		}
	}

	void MarkovChainsEnchanced::normalizeFirstChainChances() {
		firstChainNormalized = firstChain;
		double sum = 0;

		for (auto& [key, value] : firstChainNormalized) {
			sum += value;
		}

		for (auto& [key, value] : firstChainNormalized) {
			value /= sum;
		}
	}

	void MarkovChainsEnchanced::normalizeLength() {
		lengthNormalized = length;
		double sum = 0;

		for (auto& [key, value] : lengthNormalized) {
			sum += value;
		}

		for (auto& [key, value] : lengthNormalized) {
			value /= sum;
		}
	}

	void MarkovChainsEnchanced::clear() {
		dictionary.clear();
		firstChain.clear();
		length.clear();
	}

	std::string MarkovChainsEnchanced::generate(std::string first, int length) {
		normalizeChances();
		std::string word(first);

		while (word.size() < length) {
			double chance = getChance();
			double border = 0;

			for (auto& [key, value] : dictionaryNormalized[first]) {
				if (chance >= border) {
					if (chance <= (border + value)) {
						word += key;
						first.erase(0, 1);
						first += key;
						break;
					}
				}
				border += value;
			}
		}
		return word;
	}

	double MarkovChainsEnchanced::getChance() {
		return dist(random);
	}

	std::map<std::string, double>& MarkovChainsEnchanced::getFirstChain() {
		normalizeChances();
		return firstChainNormalized;
	}

	int MarkovChainsEnchanced::getRandomLength() {
		normalizeChances();

		double chance = getChance();
		double border = 0;
		int chain = 0;

		for (auto& [key, value] : lengthNormalized) {
			if (chance >= border) {
				if (chance <= (border + value)) {
					chain = key;
					break;
				}
			}

			border += value;
		}

		return chain;
	}

	std::string MarkovChainsEnchanced::getRandomChain() {
		normalizeChances();

		double chance = getChance();
		double border = 0;
		std::string chain;

		for (auto& [key, value] : firstChainNormalized) {
			if (chance >= border) {
				if (chance <= (border + value)) {
					chain = key;
					break;
				}
			}

			border += value;
		}

		return chain;
	}
}