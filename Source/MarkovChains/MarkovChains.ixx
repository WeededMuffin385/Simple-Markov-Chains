#include <filesystem>
#include <random>
#include <map>

export module Sandcore.MarkovChains;

export namespace Sandcore {
	class MarkovChains {
	public:
		MarkovChains();

		void uploadData(std::filesystem::path path);	// load data from file
		void clear();									// clear data

		std::map<char, double>& getFirstChain();		// return first symbols in initial words
		std::string generate(char first, int length);	// generate Markov chain with character [first]

		int getRandomLength();
		char getRandomChain();
	private:
		bool isNormalized = false;
		void normalizeChances();						// normalize chances to be in range [0, 1] summary

		void normalizeDictionaryChances();
		void normalizeFirstChainChances();
		void normalizeLength();

		std::map<char, std::map<char, double>> dictionary;
		std::map<char, std::map<char, double>> dictionaryNormalized;

		std::map<char, double> firstChain;
		std::map<char, double> firstChainNormalized;

		std::map<int, double> length; // return [length of name][amount of names with this length]
		std::map<int, double> lengthNormalized;

		double getChance();
		std::mt19937 random;
		std::uniform_real_distribution<double> dist;
	};
}