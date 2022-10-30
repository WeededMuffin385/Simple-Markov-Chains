#include <filesystem>
#include <random>
#include <map>

export module MarkovChains;

export class MarkovChains {
public:
	MarkovChains();

	void uploadData(std::filesystem::path path);	// load data from file
	void normalChances();							// normalize chances to be in range [0, 1] summary
	void clear();									// clear data

	std::vector<char> getFirst();					// in chains A->B return all A's
	std::string generate(char first, int length);	// generate Markov chain with character [first]

private:
	std::map<char, std::map<char, double>> dictionary;

	double getChance();
	std::mt19937 random;
	std::uniform_real_distribution<double> dist;
};