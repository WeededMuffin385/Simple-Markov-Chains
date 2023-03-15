#include <filesystem>
#include <fstream>
#include <random>

import Sandcore.MarkovChains;

namespace Sandcore {
	MarkovChains::MarkovChains() : dist(0, 1) {
		random.seed(std::random_device()());
	}

	void MarkovChains::uploadData(std::string data) {
		isNormalized = false;
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

	void MarkovChains::uploadFile(std::filesystem::path path) {
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

	void MarkovChains::deleteData() {
		dictionary.clear();
		firstChain.clear();
		length.clear();
	}

	std::string MarkovChains::generate(std::string first, int length) {
		normalizeChances();
		std::string word(first);

		while (word.size() < length) {
			double chance = getChance();
			double border = 0;

			if (dictionaryNormalized[first].empty()) break;
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
		if (word[word.size() - 1] == '-') word.erase(word.size() - 1);
		if (word[word.size() - 2] == '-') word.erase(word.size() - 2);
		if (word[word.size() - 3] == '-') word.erase(word.size() - 3);

		return word;
	}

	double MarkovChains::getChance() {
		return dist(random);
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

	std::string MarkovChains::getRandomChain() {
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