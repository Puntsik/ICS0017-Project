#include "TypingSession.h"
#include <iostream>
#include <conio.h>

const std::string RED    = "\033[31m";
const std::string GREEN  = "\033[32m";
const std::string RESET  = "\033[0m";

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


void TypingSession::mainLogic(bool& complete, std::string& userInput, const std::string& targetText) {
    if (_kbhit()) {
        char c = _getch();
        if (c == QUIT_KEY) {
            complete = true;
        }
        else if (c == BACKSPACE_KEY) {
            if (!userInput.empty()) userInput.pop_back();
        }
        else {
            userInput += c;
        }
    }
}

void TypingSession::startSession(const std::string& targetText) {
    
    bool complete = false;
    std::string userInput = "";
    hideCursor();
    initialClear();

    while (!complete) {
        resetCursor();
        std::cout << "\r";
        // loop through every character in the text the user is supposed to type
        for (int i = 0; i < targetText.size(); i++){
            // checks if the user has typed
            if (i < userInput.size()) {
                if (userInput[i] != targetText[i]){
                    userInput.pop_back();
                    std::cout << RED << targetText[i] << RESET;
                } else {
                    std::cout << GREEN << userInput[i] << RESET;
                }
                
            } else {
                std::cout << RED << targetText[i] << RESET;
            }
        }

        std::cout << clearLine();

        mainLogic(complete, userInput, targetText);
    }
    std::cout << RESET;
    showCursor();
}