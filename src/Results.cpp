#include "Results.h"
#include "TypingSession.h"
#include <iostream>
#include <iomanip>
#include "constants.h"

Results::Results(int m, int c, int d, bool q, int l) {
	mistakeCount = m;
	correctCount = c;
	duration = float(d) / 1000;
	typingAccuracy = 100 * (float(correctCount) / (correctCount + mistakeCount));
	quitStatus = q;
	textLength = l;
	WPM = (double(textLength) / duration) * 60;
}

void Results::display_results() {
	if (!quitStatus) {
		std::cout << "\033[K" << PURPLE << "\nRESULTS:\n" << RESET << std::endl;
		std::cout << "Total mistakes: " << RED << mistakeCount << RESET << std::endl;

		if (typingAccuracy >= 70) {
			std::cout << "Typing accuracy: " << GREEN << std::fixed << std::setprecision(2) << typingAccuracy << "%" << RESET << std::endl;
		}
		else if (typingAccuracy >= 50) {
			std::cout << "Typing accuracy: " << YELLOW << std::fixed << std::setprecision(2) << typingAccuracy << "%" << RESET << std::endl;
		}
		else {
			std::cout << "Typing accuracy: " << RED << std::fixed << std::setprecision(2) << typingAccuracy << "%" << RESET << std::endl;
		}
		std::cout << "Time elapsed: " << std::fixed << std::setprecision(2) << duration << " seconds" << std::endl;
		
		std::cout << "Words per minute: " << WPM << std::endl;
	}
	else {
		TypingSession::initialClear();
		std::cout << "See you next time!" << std::endl;
	}
}