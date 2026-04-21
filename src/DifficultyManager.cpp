#include "DifficultyManager.h"
#include <iostream>

int DifficultyManager::chooseDiff() {
    int level;
    std::cout << "Choose difficulty (1-Easy, 2-Medium, 3-Hard): ";
    std::cin >> level;

    int errors = 0;
    while (true) {
        if (errors == 2) {
            throw std::runtime_error("Error: bad input.");
        }
        if (std::cin.fail()) {
            errors += 1;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input, try again: ";
            std::cin >> level;
        }
        else if (level > 3 || level < 1) {
            errors += 1;
            std::cout << "Must be valid level, try again: ";
            std::cin >> level;
        }
        else {
            break;
        }
    }
    return level;
}