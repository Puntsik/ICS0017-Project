#include "TypingSession.h"
#include <iostream>
#include <conio.h>



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


void TypingSession::mainLogic(bool& complete, std::string& mainText) {
    if (_kbhit()) { // checks if the keyboard was hit
        char c = _getch();
        if (c == QUIT_KEY) { // gets the character
            complete = true;
        }
        else if (c == BACKSPACE_KEY) {
            if (!mainText.empty()) mainText.pop_back();
        }
        else {
            mainText += c;
        }
    }
}



void TypingSession::startSession(std::string& text) {
    bool complete = false;
    hideCursor();
    initialClear();

    while (!complete) {
        resetCursor();
        std::cout << text << " " << clearLine();
        mainLogic(complete, text);
    }

    showCursor();
}