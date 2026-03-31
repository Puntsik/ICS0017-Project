#include "TextManager.h"
#include <fstream>

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

    switch (difficulty) {
    case 1:
        return textLines[0];
    case 2:
        return textLines[1];
    case 3:
        return textLines[2];
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

