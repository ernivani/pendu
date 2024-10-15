#include "Network.h"
#include "Game.h"
#include <iostream>
#include <string>

int main() {
    Network network;
    std::string ip;
    std::string choice;

    for (;;) {
        std::cout << "Voulez-vous être serveur, client ou lancer un tournoi ? (s/c/t) : ";
        std::cin >> choice;

        if (choice == "s") {
            network.startServer();
            break;
        } else if (choice == "c") {
            std::cout << "Entrez l'adresse IP du serveur : (default: 127.0.0.1) ";
            std::getline(std::cin >> std::ws, ip);
            if (ip.empty()) {
                ip = "127.0.0.1";
            }
            network.startClient(ip);
            break;
        } else if (choice == "t") {
            network.startTournamentServer();
            break;
        } else {
            std::cout << "Choix invalide. Veuillez entrer 's' pour serveur, 'c' pour client, ou 't' pour tournoi." << std::endl;
        }
    }

    if (!network.isConnectedToPeer()) {
        return 1;
    }

    Game game(&network);
    game.startGame();

    return 0;
}
