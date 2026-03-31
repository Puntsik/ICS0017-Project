#pragma once
#include <string>
#include <iostream>

class Results {
private:
	int mistakeCount;
	int correctCount;
	float typingAccuracy;
public:
	Results(int m, int c);
	void display_results(); 
};