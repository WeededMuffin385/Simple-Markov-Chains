#include <string>
#include <cstdint>
#include <unordered_map>


#include <filesystem>
#include <fstream>

#include <iostream>

import Sandcore.MarkovChains;

import Sandcore.UnequalRandom;

namespace Sandcore {
	void MarkovChains::uploadData(std::string data) {
		if (data.size() >= chainLength) ++first[std::string(&data[0], &data[chainLength])];
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
		std::ifstream file(path);
		std::string data;

		while (!file.eof()) {
			file >> data;
			if (data.empty()) continue;
			uploadData(data);
		}
	}

	void MarkovChains::deleteData() {
		dictionary.clear();
		length.clear();
		first.clear();
	}

	void MarkovChains::countMax() {
		if (isMaxCounted) return; else isMaxCounted = true;

		for (auto& [key1, value1] : dictionary) {
			maxDictionary[key1] = 0;
			for (auto& [key2, value2] : value1) {
				maxDictionary[key1] += value2;
			}
		}

		maxLength = 0;
		for (auto& [key, value] : length) maxLength += value;

		maxFirst = 0;
		for (auto& [key, value] : first) maxFirst += value;
	}

	std::string MarkovChains::generate(std::string first, int length) {
		countMax();
		std::string word(first);

		while (word.size() < length) {
			if (dictionary[first].empty()) break;
			auto key = getRandomValue(dictionary[first], maxDictionary[first]);

			word += key;
			first.erase(0, 1);
			first += key;
		}
		if (word[word.size() - 1] == '-') word.erase(word.size() - 1);
		if (word[word.size() - 2] == '-') word.erase(word.size() - 2);
		if (word[word.size() - 3] == '-') word.erase(word.size() - 3);

		return word;
	}

	int MarkovChains::getRandomLength() {
		countMax();
		return getRandomValue(length, maxLength);
	}

	std::string MarkovChains::getRandomFirst() {
		countMax();
		return getRandomValue(first, maxFirst);
	}
}