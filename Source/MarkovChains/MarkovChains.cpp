#include <filesystem>
#include <fstream>
#include <random>

import Sandcore.MarkovChains;

namespace Sandcore {
	MarkovChains::MarkovChains() : dist(0, 1) {
		random.seed(std::random_device()());
	}

	void MarkovChains::uploadData(std::filesystem::path path) {
		isNormalized = false;
		std::ifstream file(path);

		while (!file.eof()) {
			std::string data;
			file >> data;
			if (data.empty()) continue;
			++firstChain[data[0]];
			++length[data.size()];
			for (int i = 0; i < data.size() - 1; ++i) {
				auto from = data[i];
				auto to = data[i + 1];
				++dictionary[from][to];
			}
		}
	}

	void MarkovChains::normalizeChances() {
		if (isNormalized) return;
		isNormalized = true;
		normalizeDictionaryChances();
		normalizeFirstChainChances();
		normalizeLength();
	}

	void MarkovChains::normalizeDictionaryChances() {
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

	void MarkovChains::normalizeFirstChainChances() {
		firstChainNormalized = firstChain;
		double sum = 0;

		for (auto& [key, value] : firstChainNormalized) {
			sum += value;
		}

		for (auto& [key, value] : firstChainNormalized) {
			value /= sum;
		}
	}

	void MarkovChains::normalizeLength() {
		lengthNormalized = length;
		double sum = 0;

		for (auto& [key, value] : lengthNormalized) {
			sum += value;
		}

		for (auto& [key, value] : lengthNormalized) {
			value /= sum;
		}
	}

	void MarkovChains::clear() {
		dictionary.clear();
		firstChain.clear();
		length.clear();
	}

	std::string MarkovChains::generate(char first, int length) {
		normalizeChances();
		std::string word; word += first;

		while (word.size() < length) {
			double chance = getChance();
			double border = 0;
			char chain = 0;

			auto last = word[word.size() - 1];

			for (auto& [key, value] : dictionaryNormalized[last]) {
				if (chance >= border) {
					if (chance <= (border + value)) {
						chain = key;
						break;
					}
				}
				border += value;
			}
			word += chain;
		}
		return word;
	}

	double MarkovChains::getChance() {
		return dist(random);
	}

	std::map<char, double>& MarkovChains::getFirstChain() {
		normalizeChances();
		return firstChainNormalized;
	}

	int MarkovChains::getRandomLength() {
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

	char MarkovChains::getRandomChain() {
		normalizeChances();

		double chance = getChance();
		double border = 0;
		char chain = 0;

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