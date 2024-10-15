#include "Network.h"
#include <iostream>
#include <regex>

#ifdef _WIN32
#include <cstring>
#else
#include <string.h>
#endif

#define PORT 54000

Network::Network() : serverSocket(-1), clientSocket(-1), isServer(false), isConnected(false) {
}

Network::~Network() {
    closeConnection();
}

bool Network::initNetwork() {
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cerr << "Échec de l'initialisation de Winsock: " << iResult << std::endl;
        return false;
    }
#endif
    return true;
}

void Network::startServer() {
    isServer = true;
    if (!initNetwork()) {
        return;
    }
    if (!createServerSocket(PORT)) {
        return;
    }
    std::cout << "En attente d'un client..." << std::endl;
    sockaddr_in clientHint;
    socklen_t clientSize = sizeof(clientHint);
    clientSocket = accept(serverSocket, (sockaddr*)&clientHint, &clientSize);
    if (clientSocket == -1) {
        std::cerr << "Échec de l'acceptation de la connexion." << std::endl;
        return;
    }
    isConnected = true;
    std::cout << "Client connecté. Le jeu peut commencer !" << std::endl;
}

void Network::startClient(const std::string& ip) {
    isServer = false;
    if (!initNetwork()) {
        return;
    }
    if (!createClientSocket(ip, PORT)) {
        return;
    }
    isConnected = true;
    std::cout << "Connecté au serveur. Le jeu peut commencer !" << std::endl;
}


void Network::startTournamentServer() {
    isServer = true;
    isTournamentMode = true;
    if (!initNetwork()) {
        return;
    }
    if (!createServerSocket(PORT)) {
        return;
    }
    std::cout << "Mode tournoi activé. En attente de joueurs..." << std::endl;
    acceptClients(2); 
    std::cout << "Nombre de joueurs suffisant. Le tournoi va commencer !" << std::endl;
    broadcast("start");
    isConnected = true;
}

void Network::acceptClients(int minClients) {
    int connectedClients = 0;
    while (connectedClients < minClients) {
        sockaddr_in clientHint;
        socklen_t clientSize = sizeof(clientHint);
        int newClientSocket = accept(serverSocket, (sockaddr*)&clientHint, &clientSize);
        if (newClientSocket == -1) {
            std::cerr << "Échec de l'acceptation de la connexion." << std::endl;
        } else {
            clientSockets.push_back(newClientSocket);
            connectedClients++;
            std::cout << "Un joueur s'est connecté. Total joueurs : " << connectedClients << std::endl;
        }
    }
}


void Network::broadcast(const std::string& message) {
    for (int sock : clientSockets) {
        sendData(sock, message);
    }
}

bool Network::createServerSocket(int port) {
    // Création du socket serveur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Impossible de créer le socket serveur." << std::endl;
        return false;
    }

    sockaddr_in serverHint;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(port);
    serverHint.sin_addr.s_addr = INADDR_ANY;

    // Liaison du socket au port
    if (bind(serverSocket, (sockaddr*)&serverHint, sizeof(serverHint)) == -1) {
        std::cerr << "Impossible de lier le socket au port." << std::endl;
        return false;
    }

    // Mise en écoute
    if (listen(serverSocket, SOMAXCONN) == -1) {
        std::cerr << "Échec de la mise en écoute." << std::endl;
        return false;
    }

    return true;
}

bool Network::createClientSocket(const std::string& ip, int port) {
    // Création du socket client
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Impossible de créer le socket client." << std::endl;
        return false;
    }

    sockaddr_in serverHint;
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverHint.sin_addr);

    // Connexion au serveur
    int connResult = connect(clientSocket, (sockaddr*)&serverHint, sizeof(serverHint));
    if (connResult == -1) {
        std::cerr << "Impossible de se connecter au serveur." << std::endl;
        return false;
    }

    // Attendre le message 'start' du serveur pour commencer
    std::string startMessage;
    if (receiveData(clientSocket, startMessage)) {
        if (startMessage == "start") {
            isConnected = true;
            std::cout << "Le tournoi commence !" << std::endl;
        } else {
            std::cerr << "Message inattendu du serveur." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Erreur lors de la réception du message du serveur." << std::endl;
        return false;
    }

    return true;
}

bool Network::sendData(int sock, const std::string& data) {
    int sendResult = send(sock, data.c_str(), data.size() + 1, 0);
    if (sendResult == -1) {
        std::cerr << "Erreur lors de l'envoi des données." << std::endl;
        return false;
    }
    return true;
}

bool Network::receiveData(int sock, std::string& data) {
    char buf[4096];
    memset(buf, 0, 4096);

    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived == -1) {
        std::cerr << "Erreur lors de la réception des données." << std::endl;
        return false;
    } else if (bytesReceived == 0) {
        std::cerr << "Connexion fermée par l'autre joueur." << std::endl;
        isConnected = false;
        return false;
    }

    data = std::string(buf, 0, bytesReceived);
    return true;
}

bool Network::receiveData(std::string& data) {
    int sock = isServer ? clientSocket : clientSocket;
    return receiveData(sock, data);
}

bool Network::sendGameState(const std::string& gameState) {
    return sendData(gameState);
}

bool Network::receiveGameState(std::string& gameState) {
    return receiveData(gameState);
}

bool Network::sendStats(const std::string& stats) {
    return sendData(stats);
}

bool Network::receiveStats(std::string& stats) {
    return receiveData(stats);
}

void Network::closeConnection() {
    if (isConnected) {
#ifdef _WIN32
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
#else
        close(clientSocket);
        close(serverSocket);
#endif
        isConnected = false;
    }
}