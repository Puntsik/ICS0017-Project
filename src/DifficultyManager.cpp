#include "DifficultyManager.h"
#include <iostream>


int DifficultyManager::chooseDiff() {
    int level;
    std::cout << "Choose difficulty (1-Easy, 2-Medium, 3-Hard): ";
    std::cin >> level;
    return level;
}