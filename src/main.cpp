#include "Network.h"
#include "Game.h"
#include <iostream>
#include <string>

int main() {
    Network network;
    std::string ip;
    std::string choice;

    for (;;) {
        std::cout << "Voulez-vous être serveur ou client ? (s/c) : ";
        std::cin >> choice;

        if (choice == "s") {
            network.startServer();
            break;
        } else if (choice == "c") {
            std::cout << "Entrez l'adresse IP du serveur : (default: 127.0.0.1) ";
            std::getline(std::cin >> std::ws, ip);
            std::cout << "ip: " << ip.empty() << std::endl;
            network.startClient(ip);
            break;
        } else {
            std::cout << "Choix invalide. Veuillez entrer 's' pour serveur ou 'c' pour client." << std::endl;
        }
    }

    if (!network.isConnectedToPeer()) {
        return 1;
    }

    Game game(&network);
    game.startGame();

    return 0;
}
