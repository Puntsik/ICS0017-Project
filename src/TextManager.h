#pragma once
#include <string>
#include <vector>

class TextManager {
private:
    std::vector<std::string> textLines;

public:
    bool loadTextFile(const std::string& filePath);
    std::string selectRandomString(int difficulty);
};