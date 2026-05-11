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

struct ServerProgress {
    float progress;
    int typedChars;
    bool completed;
};

class Client {
private:
    static constexpr const char* SERVER_IP = "127.0.0.1";
    static constexpr int SERVER_PORT = 8080;
    
    int clientSocket;
    std::string gameText;
    ServerProgress serverProgress;
    bool gameActive;
    
#ifdef _WIN32
    static WSADATA wsaData;
    static bool wsaInitialized;
#endif

    bool connectToServer();
    void sendMessage(const std::string& message);
    std::string receiveMessage();
    void listenForServerUpdates();

public:
    Client();
    ~Client();
    bool joinGame();
    std::string getGameText() const;
    ServerProgress getServerProgress() const;
    bool isGameActive() const;
    void sendProgress(float progress, int typedChars, bool completed);
    void endGame();
};
