#include "Network.h"
#include "Game.h"
#include <cstring>
#include <iostream>

int main(int argc, char* argv[]) {
    Network network;
    const char* defaultIp = "127.0.0.1";
    const char* ip = defaultIp;

    if (argc > 1) {
        if (std::strcmp(argv[1], "-s") == 0) {
            network.startServer();
        } else if (std::strcmp(argv[1], "-c") == 0) {
            if (argc > 2) {
                ip = argv[2];
            }
            network.startClient(ip);
        } else {
            std::cout << "Utilisation : " << argv[0] << " [-s | -c <adresse_ip>]" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Utilisation : " << argv[0] << " [-s | -c <adresse_ip>]" << std::endl;
        return 1;
    }

    if (!network.getIsConnected()) {
        return 1;
    }

    Game game(&network);
    game.startGame();

    return 0;
}
