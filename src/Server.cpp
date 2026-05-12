#include "Server.h"
#include <iostream>
#include <random>
#include <cstring>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <conio.h>

Server::Server() : serverSocket(-1), clientSocket(-1), gameActive(false) {
    clientProgress = {0.0f, 0, false};
    
    // Load text file for game
    if (!textManager.loadText("data/test.txt")) {
        std::cerr << "Warning: Failed to load text file. Game may not work properly." << std::endl;
    }
    
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
    }
#endif
}

Server::~Server() {
    if (serverSocket != -1) {
        CLOSE_SOCKET(serverSocket);
    }
#ifdef _WIN32
    WSACleanup();
#endif
}

std::string Server::generateRandomText() {
    // Random difficulty selection (1-3)
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> diffDist(1, 3);
    int difficulty = diffDist(rng);
    return textManager.selectRandom(difficulty);
}

void Server::sendMessage(const std::string& message) {
    if (clientSocket >= 0) {
        send(clientSocket, message.c_str(), message.length(), 0);
    }
}

std::string Server::receiveMessage() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }
    return "";
}

void Server::listenForClientUpdates() {
    while (gameActive) {
        std::string message = receiveMessage();
        if (message.empty()) {
            gameActive = false;
            break;
        }

        if (message.find("PROGRESS:") == 0) {
            std::istringstream iss(message.substr(9));
            float progress;
            int typedChars;
            int completed;
            
            if (iss >> progress >> typedChars >> completed) {
                std::lock_guard<std::mutex> lock(clientProgressMutex);
                clientProgress.progress = progress;
                clientProgress.typedChars = typedChars;
                clientProgress.completed = (completed != 0);
            }
        }
        else if (message.find("GAME_END") == 0) {
            gameActive = false;
        }
    }
}

ClientProgress Server::getClientProgress() const {
    std::lock_guard<std::mutex> lock(clientProgressMutex);
    return clientProgress;
}

void Server::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        CLOSE_SOCKET(serverSocket);
        return;
    }

    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        CLOSE_SOCKET(serverSocket);
        return;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        std::cout << "\nWaiting for a player..." << std::endl;
        
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::cout << "Player connected! Starting game..." << std::endl;
        playGame();
        
        CLOSE_SOCKET(clientSocket);
        clientSocket = -1;
    }
}

void Server::playGame() {
    // Wait for client to join
    std::string joinMsg = receiveMessage();
    if (joinMsg != "JOIN_GAME") {
        std::cerr << "Invalid join message" << std::endl;
        return;
    }

    // Pick a random text and send to client
    gameText = generateRandomText();
    sendMessage("GAME_START:" + gameText);

    gameActive = true;
    clientProgress = {0.0f, 0, false};

    // Start listening for client progress in background
    std::thread listenerThread(&Server::listenForClientUpdates, this);

    // Server game loop using same display logic as client
    bool complete = false;
    bool quit = false;
    std::string activeInput = "";
    std::string fullUserInput = "";
    
    // Hide cursor and clear screen
    std::cout << "\033[?25l\033[2J\033[H";

    auto start = std::chrono::high_resolution_clock::now();
    const int barWidth = 20;

    while (!complete && !quit && gameActive) {
        // Move cursor to top
        std::cout << "\033[H";

        int head = 0;

        if (activeInput == gameText) {
            complete = true;
            // Display all text in green
            for (int i = 0; i < gameText.length(); ++i) {
                std::cout << "\033[32m" << gameText[i] << "\033[0m";
            }
            std::cout << "\033[2J";
            std::cout << "\033[32m\n=== SERVER COMPLETED ===" << "\033[0m" << std::endl;
            sendMessage("PROGRESS:1.0 " + std::to_string((int)gameText.length()) + " 1");
        }
        else {
            // Display text with color coding
            // Correctly typed characters
            while (activeInput.length() > 0 && head < activeInput.length()) {
                if (activeInput[head] == gameText[head]) {
                    std::cout << "\033[32m" << activeInput[head] << "\033[0m";
                    head += 1;
                }
                else {
                    break;
                }
            }
            
            // Incorrectly typed characters
            for (int i = head; i < activeInput.size(); ++i) {
                if (activeInput[i] == ' ') {
                    std::cout << "\033[33m_\033[0m";
                } else {
                    std::cout << "\033[33m" << activeInput[i] << "\033[0m";
                }
            }
            
            // Characters left to type
            for (int i = head; i < gameText.size(); ++i) {
                std::cout << "\033[31m" << gameText[i] << "\033[0m";
            }

            // Timer
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
            std::cout << "\n\n[SERVER TIME] " << std::fixed << std::setprecision(2) << (elapsed.count() / 1000.0) << "s" << std::endl;

            // Server progress bar
            float serverProgress = (float)head / (float)gameText.length();
            if (serverProgress > 1.0f) serverProgress = 1.0f;
            int serverPos = (int)(serverProgress * barWidth);

            std::cout << "SERVER: |";
            for (int i = 0; i < barWidth; ++i) {
                if (i == serverPos) {
                    std::cout << "\033[32mX\033[0m";
                } else {
                    std::cout << "-";
                }
            }
            std::cout << "|" << std::endl;

            // Client progress bar
            ClientProgress clientProg = getClientProgress();
            int clientPos = (int)(clientProg.progress * barWidth);
            std::cout << "CLIENT: |";
            for (int i = 0; i < barWidth; ++i) {
                if (i == clientPos) {
                    std::cout << "\033[34mX\033[0m";
                } else {
                    std::cout << "-";
                }
            }
            if (clientProg.completed) {
                std::cout << "| \033[34mCOMPLETED!\033[0m";
            } else {
                std::cout << "|";
            }
            std::cout << std::endl;

            std::cout << "\033[K";
        }

        // Get input
        if (_kbhit()) {
            char c = _getch();
            if (c == 27) { // ESC to quit
                quit = true;
            }
            else if (c == 8) { // Backspace
                if (!activeInput.empty()) {
                    activeInput.pop_back();
                    fullUserInput.pop_back();
                }
            }
            else if (c >= 32 && c < 127) { // Printable characters
                activeInput += c;
                fullUserInput += c;
                
                // Send progress to client
                float progress = (float)head / (float)gameText.length();
                if (progress > 1.0f) progress = 1.0f;
                std::string progressMsg = "PROGRESS:" + std::to_string(progress) + " " + 
                                         std::to_string(head) + " 0";
                sendMessage(progressMsg);
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    gameActive = false;
    listenerThread.join();

    // Show cursor
    std::cout << "\033[?25h";
    std::cout << "\nGame ended" << std::endl;
}
