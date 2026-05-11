#pragma once

// Windows/Unix compatibility
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSE_SOCKET closesocket
    typedef int socklen_t;
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #define CLOSE_SOCKET close
#endif

#include <string>
#include <thread>
#include <mutex>
#include "TextManager.h"

struct ClientProgress {
    float progress;
    int typedChars;
    bool completed;
};

class Server {
private:
    static constexpr int PORT = 8080;
    int serverSocket;
    int clientSocket;
    std::string gameText;
    ClientProgress clientProgress;
    bool gameActive;
    mutable std::mutex clientProgressMutex;
    TextManager textManager;
    
#ifdef _WIN32
    WSADATA wsaData;
#endif

    std::string generateRandomText();
    void sendMessage(const std::string& message);
    std::string receiveMessage();
    void playGame();
    void listenForClientUpdates();

public:
    Server();
    ~Server();
    void start();
    ClientProgress getClientProgress() const;
};


