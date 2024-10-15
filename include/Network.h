#pragma once
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

class Network {
private:
    int serverSocket;
    int clientSocket;
    bool isServer;
    bool isConnected;

    bool initNetwork();

    bool createServerSocket(int port);
    bool createClientSocket(const std::string& ip, int port);

    bool sendData(const std::string& data);
    bool receiveData(std::string& data);

public:
    Network();
    ~Network();

    void startServer();
    void startClient(const std::string& ip);

    bool sendLetter(char lettre);
    bool receiveLetter(char& lettre);
    void closeConnection();
    bool getIsConnected() const;
    bool getIsServer() const;
};
