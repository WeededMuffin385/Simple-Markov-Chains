#include <filesystem>
#include <random>
#include <map>

export module Sandcore.MarkovChains.Enchanced;

export namespace Sandcore {
	class MarkovChainsEnchanced {
	public:
		MarkovChainsEnchanced();

		void uploadData(std::filesystem::path path); // load data from file
		void clear(); // clear data

		std::map<std::string, double>& getFirstChain(); // return first symbols in initial words
		std::string generate(std::string first, int length); // generate Markov chain with character [first]

		int getRandomLength();
		std::string getRandomChain();
	private:
		int chainLength = 3;

		bool isNormalized = false;
		void normalizeChances(); // normalize chances to be in range [0, 1] summary

		void normalizeDictionaryChances();
		void normalizeFirstChainChances();
		void normalizeLastChainChances();
		void normalizeLength();

		std::map<std::string, std::map<char, double>> dictionary;
		std::map<std::string, std::map<char, double>> dictionaryNormalized;

		std::map<std::string, double> firstChain;
		std::map<std::string, double> firstChainNormalized;

		std::map<std::string, double> lastChain;
		std::map<std::string, double> lastChainNormalized;

		std::map<int, double> length; // return [length of name][amount of names with this length]
		std::map<int, double> lengthNormalized;

		double getChance();
		std::mt19937 random;
		std::uniform_real_distribution<double> dist;
	};
}