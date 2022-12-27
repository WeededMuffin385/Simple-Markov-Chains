#include <filesystem>
#include <iostream>
#include <fstream>

import Sandcore.MarkovChains;

using namespace Sandcore;

int main() {
	setlocale(LC_ALL, "");

	MarkovChains markovChains;

	markovChains.uploadData("C:/Workbench/Markov Chains/human.txt");

	auto& arr = markovChains.getFirstChain();

	for (char c = 'A'; c <= 'Z'; ++c) {
		std::cout << markovChains.generate(markovChains.getRandomChain(), markovChains.getRandomLength()) << "\n";
	}
}