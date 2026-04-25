#include "TextManager.h"
#include <fstream>
#include <random>
#include <iostream>

bool TextManager::loadText(const std::string& filePath) {
    // for testing; to be removed from final release
    std::cout << "Trying to open: " << filePath << '\n';

    // checks that a file is loaded
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }

    textLines.clear();
    std::string line;

    while (std::getline(file, line)) {
        if (!line.empty()) {
            textLines.push_back(line);
        }
    }

    // returns 1 if no lines are read from the file
    return !textLines.empty();
}

std::string TextManager::selectRandom(int difficulty) {
    //  get random index for choosing text
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 9);
    int randomIndex = dist(gen);

    int head = 0;
    
    switch (difficulty) {
    case 1:
        head = 0 + randomIndex;
        return textLines[head];
    case 2:
        head = 10 + randomIndex;
        return textLines[head];
    case 3:
        head = 20 + randomIndex;
        return textLines[head];
    }
    return "";
}

// Deprecated methods for backward compatibility
bool TextManager::loadTextFile(const std::string& filePath) {
    return loadText(filePath);
}

std::string TextManager::selectRandomString(int difficulty) {
    return selectRandom(difficulty);
}

