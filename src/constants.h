#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string.h>
#include <iostream>
#include <string_view>
#include <string>
using namespace std;

constexpr int DIFFICULTY_EASY = 1;
constexpr int DIFFICULTY_MEDIUM = 2; 
constexpr int DIFFICULTY_HARD = 3;
constexpr int MIN_ASCII = 32;
constexpr int MAX_ASCII = 126; 
constexpr int BACKSPACE_WIN = 8; 
constexpr int BACKSPACE_POSIX = 127; 
constexpr int WORDS_PER_UNIT = 5.0;
constexpr const char* ANSI_GREEN = "\033[32m" ;
constexpr const char* ANSI_RED = "\033[31m" ;
constexpr const char* ANSI_RESET = "\033[0m";
constexpr const char* ANSI_HIDE_CURSOR = "\033[?25l";
constexpr const char* ANSI_SHOW_CURSOR = "\033[?25h" ;
#endif