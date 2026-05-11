#include "Client.h"
#include <cstring>
#include <iostream>
#include <thread>
#include <sstream>

#ifdef _WIN32
WSADATA Client::wsaData = {};
bool Client::wsaInitialized = false;
#endif

Client::Client() : clientSocket(-1), gameActive(false) {
    serverProgress = {0.0f, 0, false};
    
#ifdef _WIN32
    if (!wsaInitialized) {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
        }
        wsaInitialized = true;
    }
#endif
}

Client::~Client() {
    endGame();
}

bool Client::connectToServer() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    if (serverAddress.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid address" << std::endl;
        return false;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return false;
    }

    return true;
}

void Client::sendMessage(const std::string& message) {
    if (clientSocket >= 0) {
        send(clientSocket, message.c_str(), message.length(), 0);
    }
}

std::string Client::receiveMessage() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }
    return "";
}

void Client::listenForServerUpdates() {
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
                serverProgress.progress = progress;
                serverProgress.typedChars = typedChars;
                serverProgress.completed = (completed != 0);
            }
        }
    }
}

bool Client::joinGame() {
    if (!connectToServer()) {
        return false;
    }

    sendMessage("JOIN_GAME");
    std::string response = receiveMessage();
    
    if (response.find("GAME_START:") == 0) {
        gameText = response.substr(11);
        gameActive = true;
        std::thread(&Client::listenForServerUpdates, this).detach();
        return true;
    }

    return false;
}

void Client::sendProgress(float progress, int typedChars, bool completed) {
    std::string message = "PROGRESS:" + std::to_string(progress) + " " + 
                          std::to_string(typedChars) + " " + 
                          std::to_string(completed ? 1 : 0);
    sendMessage(message);
}

std::string Client::getGameText() const {
    return gameText;
}

ServerProgress Client::getServerProgress() const {
    return serverProgress;
}

bool Client::isGameActive() const {
    return gameActive;
}

void Client::endGame() {
    if (clientSocket >= 0) {
        sendMessage("GAME_END");
        CLOSE_SOCKET(clientSocket);
        clientSocket = -1;
    }
    gameActive = false;
}
