#include <filesystem>
#include <iostream>
#include <fstream>

import Sandcore.MarkovChains.Enchanced;

using namespace Sandcore;

int main() {
	setlocale(LC_ALL, "");

	MarkovChainsEnchanced markovChains;

	markovChains.uploadData("C:/Workbench/Markov Chains/human.txt");

	auto& arr = markovChains.getFirstChain();

	for (int i = 0; i < 10; ++i) {
		std::cout << markovChains.generate(markovChains.getRandomChain(), markovChains.getRandomLength()) << "\n";
	}
}