#include <iostream>

import MarkovChains;

int main() {
	setlocale(LC_ALL, "");

	MarkovChains markovChains;

	markovChains.uploadData("C:/Workbench/Markov Chains/names.txt");
	markovChains.normalChances();

	for (auto& c : markovChains.getFirst()) {
		std::cout << markovChains.generate(c, 10) << "\n";
	}
}