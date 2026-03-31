#pragma once
#include <string>

/**
 * @interface ITextProvider
 * @brief Abstract interface for text providers
 * 
 * This interface allows different implementations for text loading and selection,
 * enabling substitution of alternative providers (e.g., TextManager, RandomWordGenerator)
 */
class ITextProvider {
public:
    virtual ~ITextProvider() = default;

    /**
     * @brief Load text from a source
     * @param filePath Path to the text file (implementation-dependent)
     * @return true if text was loaded successfully, false otherwise
     */
    virtual bool loadText(const std::string& filePath) = 0;

    /**
     * @brief Select and return a string based on difficulty level
     * @param difficulty Difficulty level (1-3 or implementation-specific)
     * @return The selected string
     */
    virtual std::string selectRandom(int difficulty) = 0;
};
