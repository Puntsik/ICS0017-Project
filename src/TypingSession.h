#pragma once
#include <string>
#include <chrono>

class TypingSession {
private:
    static constexpr int QUIT_KEY = 27;
    static constexpr int BACKSPACE_KEY = 8;

    // code control ANSI to work with the cursors
    void hideCursor();
    void showCursor();
    void resetCursor();
    std::string clearLine();
    void initialClear();

    void mainLogic(bool& complete, std::string& activeInput, std::string& fullUserInput, const std::string& targetText);


    //store results to send to Results object
    int mistakeCount;
    int correctCount;
    std::chrono::milliseconds duration;

public:
    // starts the session with a reference string, in this case a text (read-only)
    
    static void handleSigint(int signal);
    void startSession(const std::string& text);
    
    int getMistakes();
    int getCorrect();
    int getDuration();
};