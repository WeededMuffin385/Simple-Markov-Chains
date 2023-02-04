#include <filesystem>
#include <random>
#include <map>
#include <unordered_map>

export module Sandcore.MarkovChains;

export namespace Sandcore {
	class MarkovChains {
	public:
		MarkovChains();

		void uploadData(std::filesystem::path path); // load data from file
		void deleteData(); // clear data

		std::string generate(std::string first, int length); // generate Markov chain with character [first]

		int getRandomLength();
		std::string getRandomChain();
	private:
		int chainLength = 3;

		bool isNormalized = false;
		void normalizeChances(); // normalize chances to be in range [0, 1] summary

		void normalizeDictionaryChances();
		void normalizeFirstChainChances();
		void normalizeLength();

		std::unordered_map<std::string, std::map<char, double>> dictionary;
		std::unordered_map<std::string, std::map<char, double>> dictionaryNormalized;

		std::unordered_map<std::string, double> firstChain;
		std::unordered_map<std::string, double> firstChainNormalized;

		std::unordered_map<int, double> length; // return [length of name][amount of names with this length]
		std::unordered_map<int, double> lengthNormalized;

		double getChance();
		std::mt19937 random;
		std::uniform_real_distribution<double> dist;
	};
}