#pragma once
#include <string>
#include <iostream>

class Results {
private:
	int mistakeCount;
	int correctCount;
	float duration;
	float typingAccuracy;
public:
	Results(int m, int c, int d);
	void display_results(); 
};