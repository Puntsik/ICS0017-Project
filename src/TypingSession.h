#pragma once
#include <string>
#include <chrono>

class Client;

class TypingSession {
private:
    static constexpr int QUIT_KEY = 27;
    static constexpr int BACKSPACE_KEY = 8;
    static constexpr int ENTER_KEY = 13;

    // feat: add typingsession multiplayer 
    void mainLogic(bool& complete, bool& quit, std::string& activeInput, std::string& fullUserInput, const std::string& targetText);
    void mainLogicMultiplayer(bool& complete, bool& quit, std::string& activeInput, std::string& fullUserInput, const std::string& targetText, Client* client);

    void handleInput(bool& complete, bool& quit, std::string& activeInput, std::string& fullUserInput);
    void drawProgressBar(int position, int barWidth, bool isOpponent = false, bool opponentCompleted = false);
    void drawTypingDisplay(const std::string& activeInput, const std::string& targetText, int& head);
    
    //store results to send to Results object
    int mistakeCount;
    int correctCount;
    std::chrono::milliseconds duration;
    bool quitStatus = false;
    int textLength;

public:
    // starts the session with a reference string, in this case a text (read-only)
    
    static void handleSigint(int signal);
    void startSession(const std::string& text);
    void startMultiplayerSession(const std::string& text, Client* client);
    
    int getMistakes();
    int getCorrect();
    int getDuration();
    int calculateTextLength(const std::string& text);
    bool getQuitStatus();
    int getTextLength();

    // code control ANSI to work with the cursors
    static void hideCursor();
    static void showCursor();
    static void resetCursor();
    static std::string clearLine();
    static void initialClear();
    static void cursorDown();
};