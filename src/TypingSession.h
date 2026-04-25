#pragma once
#include <string>
#include <chrono>

class TypingSession {
private:
    static constexpr int QUIT_KEY = 27;
    static constexpr int BACKSPACE_KEY = 8;
    static constexpr int ENTER_KEY = 13;

    

    void mainLogic(bool& complete, bool& quit, std::string& activeInput, std::string& fullUserInput, const std::string& targetText);


    //store results to send to Results object
    int mistakeCount;
    int correctCount;
    std::chrono::milliseconds duration;
    bool quitStatus = false;

public:
    // starts the session with a reference string, in this case a text (read-only)
    
    static void handleSigint(int signal);
    void startSession(const std::string& text);
    
    int getMistakes();
    int getCorrect();
    int getDuration();
    bool getQuitStatus();

    // code control ANSI to work with the cursors
    static void hideCursor();
    static void showCursor();
    static void resetCursor();
    static std::string clearLine();
    static void initialClear();
    static void cursorDown();
};