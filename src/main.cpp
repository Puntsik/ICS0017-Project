#include <iostream>
#include <string>
#include <csignal>
#include "TypingSession.h"
#include "TextManager.h"
#include "DifficultyManager.h"
#include "Results.h"
#include "utils.h"

int main(int argc, const char* argv[]) {
    DifficultyManager diffManager;
    TextManager textManager;
    TypingSession session;
    std::signal(SIGINT, session.handleSigint);

    try {
        if (!textManager.loadText("data/test.txt"))
            throw std::runtime_error("Failed to load file.");

        int difficulty = diffManager.chooseDiff();
        std::string text = textManager.selectRandom(difficulty);

        session.startSession(text);

        Results newresult(session.getMistakes(), session.getCorrect(),
                          session.getDuration(), session.getQuitStatus(),
                          session.getTextLength());
        newresult.display_results();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    charToContinue();

    return 0;
}