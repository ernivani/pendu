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

    // Initialisation du réseau
    bool initNetwork();

    // Gestion des sockets
    bool createServerSocket(int port);
    bool createClientSocket(const std::string& ip, int port);

    // Envoi et réception de données
    bool sendData(const std::string& data);
    bool receiveData(std::string& data);

public:
    Network();
    ~Network();

    void startServer();
    void startClient(const std::string& ip);

    bool sendGameState(const std::string& gameState);
    bool receiveGameState(std::string& gameState);

    bool sendStats(const std::string& stats);
    bool receiveStats(std::string& stats);

    void closeConnection();

    bool isConnectedToPeer() const { return isConnected; }
    bool isServerMode() const { return isServer; }
};
