#pragma once
#include <string>
#include <vector>
#include "ITextProvider.h"

class TextManager : public ITextProvider {
private:
    std::vector<std::string> textLines;

public:
    // Implementation of ITextProvider interface
    bool loadText(const std::string& filePath) override;
    std::string selectRandom(int difficulty) override;

    // Deprecated: use loadText() instead
    bool loadTextFile(const std::string& filePath);
    // Deprecated: use selectRandom() instead
    std::string selectRandomString(int difficulty);
};