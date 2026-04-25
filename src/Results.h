#pragma once
#include <string>
#include <iostream>

class Results {
private:
	int mistakeCount;
	int correctCount;
	float duration;
	float typingAccuracy;
	bool quitStatus;
public:
	Results(int m, int c, int d, bool q);
	void display_results(); 
};