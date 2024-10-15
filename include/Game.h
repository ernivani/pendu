#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include "Mot.h"
#include "Joueur.h"
#include "Network.h"
#include <map>

class Game {
private:
    std::string motADeviner;
    Mot* mot;
    Joueur* joueur;
    int nombreCoups;
    int motsDevinés;

    Network* network;

    std::string opponentMotCache;
    int opponentTentativesRestantes;
    std::vector<char> opponentLettresProposees;


    bool isTournament;
    int playerID;
    std::map<int, int> scores; // ID du joueur et nombre de mots devinés
    int currentPlayerID;

    std::string sauvegardePath;
    std::string statistiquesPath;
    std::string motsPath;

    std::string getSaveDirectory();
    void createDirectoryIfNotExists(const std::string& path);

    std::string encode(const std::string& data);
    std::string decode(const std::string& data);

    void envoyerEtatJeu();
    void recevoirEtatJeu();

public:
    Game(Network* net);
    ~Game();

    void initGame();
    void startGame();
    void afficherEtatJeu();
    void afficherEtatJeuAdversaire();
    char demanderLettre();
    bool estFinJeu();
    std::string choisirMotAleatoire();

    void chargerStatistiques();
    void sauvegarderStatistiques();
    void afficherStatistiques();

    void playTournament();
    void afficherClassement();
};
