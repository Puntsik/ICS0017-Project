#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include "utils.h"

void charToContinue() {
	std::cout << "\nPress any character to continue\n" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(2));
	_getch();
}
