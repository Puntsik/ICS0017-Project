#include "TypingSession.h"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <iomanip>

const std::string RED    = "\033[31m";
const std::string GREEN  = "\033[32m";
const std::string RESET  = "\033[0m";
const std::string YELLOW = "\033[33m";


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
    while (!complete && !quit) {
        int displayTextLength = targetText.length();
        resetCursor();
        std::cout << "\r";

        // check if text has been completed by user
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
    }

    showCursor();
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