#include <iostream>

class TypingSession {
public:
    void startSession(std::string text){
        std::cout<< text <<std::endl;
    }
};

int main(int argc, const char * argv[]){
    TypingSession main;
    main.startSession("test");
    return 0;
}