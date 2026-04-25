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
	int textLength;
	double WPM;
public:
	Results(int m, int c, int d, bool q, int l);
	void display_results(); 
};