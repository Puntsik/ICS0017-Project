#include "Results.h"
#include <iostream>
#include <iomanip>

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RESET = "\033[0m";

Results::Results(int m, int c, int d) {
	mistakeCount = m;
	correctCount = c;
	duration = float(d) / 1000;
	typingAccuracy = 100 * (float(correctCount) / (correctCount + mistakeCount));
}

void Results::display_results() {
	std::cout << "\nResults:\n" << std::endl;
	std::cout << "Total mistyped characters: " << mistakeCount << std::endl;

	if (typingAccuracy >= 70) {
		std::cout << "Typing accuracy: " << GREEN << std::fixed << std::setprecision(2) << typingAccuracy << "%" << RESET << std::endl;
	}
	else if (typingAccuracy >= 50) {
		std::cout << "Typing accuracy: " << YELLOW << std::fixed << std::setprecision(2) << typingAccuracy << "%" << RESET << std::endl;
	}
	else {
		std::cout << "Typing accuracy: " << RED << std::fixed << std::setprecision(2) << typingAccuracy << "%" << RESET << std::endl;
	}
	
	std::cout << "Time elapsed: " << std::fixed << std::setprecision(2) << duration << "seconds" << std::endl;
}