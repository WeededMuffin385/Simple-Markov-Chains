#include <filesystem>
#include <iostream>
#include <fstream>

import Sandcore.MarkovChains;

using namespace Sandcore;

int main() {
	setlocale(LC_ALL, "");

	MarkovChains markovChains;

	markovChains.uploadData("C:/Users/Mi/Documents/GitHub/Simple-Markov-Chains/Data/human names.txt");

	for (int i = 0; i < 100; ++i) {
		std::cout << markovChains.generate(markovChains.getRandomChain(), markovChains.getRandomLength()) << "\n";
	}
}