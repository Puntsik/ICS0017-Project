#include "TypingSession.h"
#include <iostream>
#include <conio.h>

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


void TypingSession::mainLogic(bool& complete, std::string& activeInput, std::string& fullUserInput, const std::string& targetText) {
    if (_kbhit()) {
        char c = _getch();
        if (c == QUIT_KEY) {
            complete = true;
        }
        else if (c == BACKSPACE_KEY) {
            if (!activeInput.empty()) activeInput.pop_back();
        }
        else {
            activeInput += c;
            fullUserInput += c;
        }
    }
}

void TypingSession::startSession(const std::string& targetText) {
    
    bool complete = false;
    std::string activeInput = "";
    std::string fullUserInput = "";
    hideCursor();
    initialClear();

    while (!complete) {
        resetCursor();
        std::cout << "\r";

        // check if text has been completed by user
        if (activeInput == targetText) {
            complete = true;
            for (int i = 0; i < targetText.length(); ++i) {
                std::cout << GREEN << targetText[i] << RESET;
            }
            std::cout << "\n" << GREEN << "COMPLETED" << RESET << std::endl;

        }
        else {
            // typing display

            int head = 0;
            //correctly typed characters
            while (true) {
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
                std::cout << YELLOW << activeInput[i] << RESET;
            }
            // characters left to type
            for (int i = head; i < targetText.size(); ++i) {
                std::cout << RED << targetText[i] << RESET;
            }
        }
        
        std::cout << clearLine();
        if (!complete) mainLogic(complete, activeInput, fullUserInput, targetText);
    }
    std::cout << RESET;
    
    mistakeCount = fullUserInput.length() - targetText.length();
    correctCount = targetText.length();
    
    showCursor();
}

int TypingSession::getMistakes() {
    return mistakeCount;
}

int TypingSession::getCorrect() {
    return correctCount;
}