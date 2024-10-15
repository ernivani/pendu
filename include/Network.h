#pragma once
#include <string>
#include <vector>

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
    bool isTournamentMode;

    std::vector<int> clientSockets; 

    bool initNetwork();

    bool createServerSocket(int port);
    bool createClientSocket(const std::string& ip, int port);

    bool sendData(const std::string &data);

    bool receiveData(std::string &data);

    bool sendData(int sock, const std::string& data);
    bool receiveData(int sock, std::string& data);

public:
    Network();
    ~Network();

    void startServer();
    void startClient(const std::string& ip);

    void startTournamentServer();
    void acceptClients(int minClients);
    void broadcast(const std::string& message);

    bool sendGameState(const std::string& gameState);
    bool receiveGameState(std::string& gameState);

    bool sendStats(const std::string& stats);
    bool receiveStats(std::string& stats);

    void closeConnection();

    bool isConnectedToPeer() const { return isConnected; }
    bool isServerMode() const { return isServer; }
    bool isTournament() const { return isTournamentMode; }
};
