#include <iostream>
#include <string>
#include <csignal>
#include "TypingSession.h"
#include "TextManager.h"
#include "DifficultyManager.h"
#include "Results.h"

int main(int argc, const char* argv[]) {
    // init of the managers
    DifficultyManager diffManager;
    TextManager textManager;
    TypingSession session;
    std::signal(SIGINT, session.handleSigint);

    // currently just reads the test.txt with 3 lines; 1 for each possible difficulty selection.
    if (textManager.loadTextFile("data/test.txt")) {
        std::cout << "File loaded successfully!\n";
    }
    else {
        std::cout << "Failed to load file.\n";
        return 0;
    }

    // Gives different line depending of chosen difficulty
    int difficulty;
    try {
        difficulty = diffManager.chooseDiff();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    std::string text = textManager.selectRandomString(difficulty);

    // the most important part
    session.startSession(text);

    Results newresult(session.getMistakes(), session.getCorrect(), session.getDuration(), session.getQuitStatus(), session.getTextLength());
    newresult.display_results();

    return 0;
}
