module;
#include <string>
#include <cstdint>
#include <unordered_map>

#include <filesystem>
#include <fstream>

export module Sandcore.MarkovChains;


export namespace Sandcore {
	class MarkovChains {
	public:
		void uploadData(std::string data);
		void uploadFile(std::filesystem::path path);

		void deleteData();

		std::string generate(std::string first, int length);

		int getRandomLength();
		std::string getRandomFirst();
	private:
		int chainLength = 3;
		bool isMaxCounted = false;
		void countMax();

		std::unordered_map <std::string, std::unordered_map<char, std::uint64_t>> dictionary;
		std::unordered_map <std::string, std::uint64_t> first;
		std::unordered_map <int, std::uint64_t> length;

		std::unordered_map <std::string, std::uint64_t> maxDictionary;
		std::uint64_t maxLength;
		std::uint64_t maxFirst;
	};
}