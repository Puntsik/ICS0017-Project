#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

std::string readRandomSentence(int Index, std::string filePath){
    std::string line;
    int random = 0;
    int numOfLines = 0;
    std::ifstream File(filePath);
    
        srand(time(0));
        random = rand() % Index;

    while(getline(File, line))
    {
        ++numOfLines;

        if(numOfLines == random)
        {
            return line;
        }
    }
}


class TypingSession {
private:
    static constexpr int QUIT_KEY = 27; 
    static constexpr int BACKSPACE_KEY = 8;

    void hideCursor() { std::cout << "\033[?25l"; }
    void showCursor() { std::cout << "\033[?25h"; }
    void resetCursor() { std::cout << "\033[H"; }
    std::string clearLine() { return "\033[K"; }
    void initialClear() { std::cout << "\033[2J"; }
    
    void mainLogic(bool& complete, std::string& mainText){
        if (_kbhit()) { 
            char c = _getch(); 
            if (c == QUIT_KEY) {
                complete = true;
            } else if (c == BACKSPACE_KEY){
                if (!mainText.empty()) mainText.pop_back();
            } else{
                mainText+=c;
            }
    }
}
    
public:
    void startSession(std::string &text){
        bool complete = false;
        hideCursor(); 
        initialClear();
        while (!complete){
            resetCursor();
            std::cout<< text << " " << clearLine();
            mainLogic(complete, text);
            }
        showCursor();
    }
};

class DifficultyManager {
public:
    int chooseDiff(){
        int level;
        std::cout << "Choose difficulty (1-Easy, 2-Medium, 3-Hard): ";
        std::cin >> level;
        return level;
    }  
};

class TextManager {
    public:
    std::string selectRandomString(int difficulty){
        switch (difficulty){
            case 1:
            return "test";
            case 2:
            return "test";
            case 3:
            return "test";
        }
        return "";
    }
};

int main(int argc, const char * argv[]){
    DifficultyManager diffManager;
    TextManager textManager;
    TypingSession session;
    std::cout << "\033[31mAcest text este rosu!\033[0m" << std::endl;

    int difficulty = diffManager.chooseDiff();
    std::string text = textManager.selectRandomString(difficulty);

    session.startSession(text);

    return 0;
}