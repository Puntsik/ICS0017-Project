#include "TypingSession.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include "constants.h"
#include "utils.h"

#include <iomanip>
#include <random>

void TypingSession::handleSigint(int signal) {
    std::cout << RESET;   
    std::cout << "\033[?25h"; 
    std::cout << std::endl;
    std::_Exit(0);        
}

void TypingSession::hideCursor() {
	std::cout << "\033[?25l";
}

void TypingSession::showCursor() {
	std::cout << "\033[?25h";
}

void TypingSession::resetCursor() {
	std::cout << "\033[H";
}

std::string TypingSession::clearLine() {
	return "\033[K";
}

void TypingSession::initialClear() {
	std::cout << "\033[2J";
}

void TypingSession::cursorDown() {
    std::cout << "\033[J";
}


void TypingSession::mainLogic(bool& complete, bool& quit, std::string& activeInput, std::string& fullUserInput, const std::string& targetText) {
    if (_kbhit()) {
        char c = _getch();
        if (c == QUIT_KEY) {
            quit = true;
        }
        else if (c == BACKSPACE_KEY) {
            if (!activeInput.empty()) activeInput.pop_back();
        }
        else if (c == ENTER_KEY) {
            return;
        }
        else {
            activeInput += c;
            fullUserInput += c;
        }
    }
}

void TypingSession::startSession(const std::string& targetText) {
    
    bool complete = false;
    bool quit = false;
    std::string activeInput = "";
    std::string fullUserInput = "";
    hideCursor();
    initialClear();

    //  start timer
    auto start = std::chrono::high_resolution_clock::now();

    //  counter to clear line every 100 loops
    int clearCounter = 0;


    // Random color generator setup (choose one color for the whole session)
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> colorDist(31, 36); // ANSI 31-36: red, green, yellow, blue, magenta, cyan
    int chosenColorCode = colorDist(rng);
    std::string chosenColor = "\033[" + std::to_string(chosenColorCode) + "m";

    while (!complete && !quit) {
        int displayTextLength = targetText.length();
        resetCursor();
        std::cout << "\r";

        if (activeInput == targetText) {
            complete = true;
            for (int i = 0; i < targetText.length(); ++i) {
                std::cout << GREEN << targetText[i] << RESET;
            }
            initialClear();
            std::cout << GREEN << "\nCOMPLETED" << RESET << std::endl;

        }
        else {              
            // typing display:
            int head = 0;

            //  correctly typed characters
            while (activeInput.length() > 0 && head < activeInput.length()) {
                if (activeInput[head] == targetText[head]) {
                    std::cout << GREEN << activeInput[head] << RESET;
                    head += 1;
                }
                else {
                    break;
                }
            }
            
            // incorrectly typed characters
            for (int i = head; i < activeInput.size(); ++i) {
                if (activeInput[i] == ' ') {
                    std::cout << YELLOW << '_' << RESET;
                    displayTextLength++;
                    continue;
                }
                std::cout << YELLOW << activeInput[i] << RESET;
            }
            // characters left to type
            for (int i = head; i < targetText.size(); ++i) {
                std::cout << RED << targetText[i] << RESET;
            }

            // clear screen and shit
            int currentDisplayTextLength = activeInput.size() - head + 1 + targetText.size();
            if (displayTextLength < currentDisplayTextLength) {
                if (displayTextLength > targetText.size()) {
                    displayTextLength -= 1;
                }
                //  TEMPORARY SOLUTION TO ONLY END OF FIRST LINE GETTING FIXED
                //  should separate text into fixed size lines for release 4
                //  clear 5 lines just in case
                for (int i = 0; i < 5; ++i) {
                    std::cout << clearLine();
                    cursorDown();
                }
            }

            // display timer when typing
            auto now = std::chrono::high_resolution_clock::now();
            auto currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

            //  print current time elapsed
            float currentDuration = currentTime.count();
            std::cout << "\n\nTime: " << std::fixed << std::setprecision(2) << currentDuration / 1000 << std::endl;

            // progress bar with one random color (chosen at session start)
            int barWidth = 20;
            float progress = (float)head / (float)targetText.length();
            if (progress > 1.0f) progress = 1.0f;
            int pos = (int)(progress * barWidth);
            std::cout << "|";
            for (int i = 0; i < barWidth; ++i) {
                if (i == pos) {
                    std::cout << chosenColor << "X" << RESET;
                } else {
                    std::cout << "-" << RESET;
                }
            }
            std::cout << "|\n";
        }
        
        
        std::cout << clearLine();
        if (!complete && !quit) mainLogic(complete, quit, activeInput, fullUserInput, targetText);
    }
    


    //  end timer
    auto end = std::chrono::high_resolution_clock::now();
    //duration in ms
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << RESET;
    if (quit) {
        std::cout << clearLine();
        quitStatus = true;
    }
    else {
        mistakeCount = fullUserInput.length() - targetText.length();
        correctCount = targetText.length();
        textLength = calculateTextLength(targetText);
    }

    showCursor();
}

int TypingSession::calculateTextLength(const std::string& text) {
    int length = 1;
    for (int i = 0; i < text.length(); ++i) {
        if (text[i] == ' ') {
            length++;
        }
    }
    return length;
}

int TypingSession::getMistakes() {
    return mistakeCount;
}

int TypingSession::getCorrect() {
    return correctCount;
}

int TypingSession::getDuration() {
    return duration.count();
}

bool TypingSession::getQuitStatus() {
    return quitStatus;
}

int TypingSession::getTextLength() {
    return textLength;
}