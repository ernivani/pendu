#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include "Mot.h"
#include "Joueur.h"
#include "Network.h"

class Game {
private:
    std::string motADeviner;
    Mot* mot;
    Joueur* joueur;
    int nombreCoups;
    int motsDevinés;

    Network* network;
    bool isMyTurn;

    std::string sauvegardePath;
    std::string statistiquesPath;
    std::string motsPath;

    std::string getSaveDirectory();
    void createDirectoryIfNotExists(const std::string& path);

    std::string encode(const std::string& data);
    std::string decode(const std::string& data);

public:
    Game(Network* net);
    ~Game();

    void initGame();
    void startGame();
    void afficherEtatJeu();
    char demanderLettre();
    bool estFinJeu();
    std::string choisirMotAleatoire();

    void sauvegarderPartie();
    bool chargerPartie();

    void chargerStatistiques();
    void sauvegarderStatistiques();
    void afficherStatistiques();
};
