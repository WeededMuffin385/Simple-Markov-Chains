#include <filesystem>
#include <fstream>
#include <random>

import MarkovChains;

MarkovChains::MarkovChains() : dist(0, 1) {
	random.seed(std::random_device()());
}

void MarkovChains::uploadData(std::filesystem::path path) {
	std::ifstream file(path);

	while (!file.eof()) {
		std::string data;
		file >> data;
		if (data.empty()) continue;

		for (int i = 0; i < data.size() - 1; ++i) {
			auto from = data[i];
			auto to = data[i + 1];
			++dictionary[from][to];
		}
	}
}

void MarkovChains::normalChances() {
	for (auto& [key, value] : dictionary) {
		double sum = 0;

		for (auto& [key2, value2] : value) {
			sum += value2;
		}

		for (auto& [key2, value2] : value) {
			value2 /= sum;
		}
	}
}

void MarkovChains::clear() {
	dictionary.clear();
}

std::string MarkovChains::generate(char first, int length) {
	std::string word; word += first;

	while (word.size() < length) {
		double chance = getChance();
		char chain = 0;
		double border = 0;

		auto last = word[word.size() - 1];

		for (auto& [key, value] : dictionary[last]) {
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

std::vector<char> MarkovChains::getFirst() {
	std::vector<char> firstChains;
	firstChains.reserve(dictionary.size());

	for (auto& [key, value] : dictionary) firstChains.push_back(key);

	return firstChains;
}