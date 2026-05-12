#include <iostream>
#include <string>
#include <csignal>
#include "TypingSession.h"
#include "TextManager.h"
#include "DifficultyManager.h"
#include "Results.h"
#include "utils.h"
#include "Client.h"
#include "Server.h"

// FEAT: add function to start the server
void runServer() {
    Server server;
    server.start();
}

void runSinglePlayer() {
    DifficultyManager diffManager;
    TextManager textManager;
    TypingSession session;
    std::signal(SIGINT, session.handleSigint);

    try {
        if (!textManager.loadText("data/test.txt"))
            throw std::runtime_error("Failed to load file.");

        int difficulty = diffManager.chooseDiff();
        std::string text = textManager.selectRandom(difficulty);

        session.startSession(text);

        Results newresult(session.getMistakes(), session.getCorrect(),
                          session.getDuration(), session.getQuitStatus(),
                          session.getTextLength());
        newresult.display_results();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    charToContinue();
}

// FEAT: add multiplayer component
void runMultiplayer() {
    Client client;
    TypingSession session;
    std::signal(SIGINT, session.handleSigint);
    // error handling to connect to the server
    try {
        std::cout << "Connecting to game server..." << std::endl;
        if (!client.joinGame()) {
            std::cout << "Failed to connect to server. Make sure it's running!" << std::endl;
            return;
        }

        std::string gameText = client.getGameText();
        std::cout << "Connected! Starting game..." << std::endl;
        std::cout << "Text: " << gameText << std::endl;

        session.startMultiplayerSession(gameText, &client);

        Results newresult(session.getMistakes(), session.getCorrect(),
                          session.getDuration(), session.getQuitStatus(),
                          session.getTextLength());
        newresult.display_results();

        client.endGame();
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    charToContinue();
}

int main(int argc, const char* argv[]) {
    std::cout << "=== TYPING GAME ===" << std::endl;
    std::cout << "1. Single Player" << std::endl;
    std::cout << "2. Multiplayer (Join Game)" << std::endl;
    std::cout << "3. Host Server (Start a game)" << std::endl;
    std::cout << "Choose mode (1-3): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();

    try {
        switch(choice) {
            case 1:
                runSinglePlayer();
                break;
            case 2:
                runMultiplayer();
                break;
            case 3:
                runServer();
                break;
            default:
                std::cout << "Invalid choice" << std::endl;
                return 1;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}