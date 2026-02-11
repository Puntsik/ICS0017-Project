#include <iostream>
#include <string>

class TypingSession {
public:
    void startSession(const std::string &text){
        std::cout<< text <<std::endl;
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
    }
};

int main(int argc, const char * argv[]){
    DifficultyManager diffManager;
    TextManager textManager;
    TypingSession session;

    int difficulty = diffManager.chooseDiff();
    std::string text = textManager.selectRandomString(difficulty);

    session.startSession(text);

    return 0;
}