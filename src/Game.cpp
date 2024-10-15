#include "Game.h"

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#endif

#include <sstream>
#include <iostream>
#include <cstring>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

Game::Game(Network* net) : nombreCoups(0), motsDevinés(0), network(net) {
    std::string saveDir = getSaveDirectory();
    sauvegardePath = saveDir + "/sauvegarde.txt";
    statistiquesPath = saveDir + "/statistiques.txt";
    motsPath = "mots.txt"; // Le fichier mots.txt est situé dans le répertoire courant

    initGame();
}

Game::~Game() {
    delete mot;
    delete joueur;
}

void Game::initGame() {
    motADeviner = choisirMotAleatoire();
    mot = new Mot(motADeviner);
    joueur = new Joueur(6);

    // Envoyer les statistiques au début du jeu
    std::string stats = std::to_string(nombreCoups) + " " + std::to_string(motsDevinés);
    network->sendStats(stats);

    // Recevoir les statistiques de l'adversaire
    std::string opponentStats;
    network->receiveStats(opponentStats);
}

std::string Game::getSaveDirectory() {
    std::string saveDir;
#ifdef _WIN32
    char path[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path) == S_OK) {
        saveDir = std::string(path) + "\\imPendu";
    } else {
        saveDir = ".";
    }
#else
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        struct passwd* pw = getpwuid(getuid());
        homeDir = pw->pw_dir;
    }
    saveDir = std::string(homeDir) + "/.local/share/imPendu";
#endif
    createDirectoryIfNotExists(saveDir);
    return saveDir;
}

void Game::createDirectoryIfNotExists(const std::string& path) {
#ifdef _WIN32
    DWORD ftyp = GetFileAttributesA(path.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        CreateDirectoryA(path.c_str(), NULL);
    }
#else
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        mkdir(path.c_str(), 0777);
    }
#endif
}

std::string Game::choisirMotAleatoire() {
    std::vector<std::string> mots;
    std::ifstream fichier(motsPath);
    std::string motLu;

    while (fichier >> motLu) {
        mots.push_back(motLu);
    }
    fichier.close();

    if (mots.empty()) {
        std::cerr << "Erreur : le fichier mots.txt est vide ou introuvable." << std::endl;
        exit(1);
    }

    srand(time(0));
    return mots[rand() % mots.size()];
}

void Game::afficherEtatJeu() {
    std::cout << "Votre mot à deviner : ";
    std::string motCache = mot->getMotCache();
    for (char c : motCache) {
        if (c != '_') {
            std::cout << GREEN << c << RESET << " ";
        } else {
            std::cout << c << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Vos tentatives restantes : " << joueur->getTentativesRestantes() << std::endl;
    std::cout << "Vos lettres proposées : ";
    for (char lettre : joueur->getLettresProposees()) {
        std::cout << lettre << " ";
    }
    std::cout << std::endl << std::endl;
}

void Game::afficherEtatJeuAdversaire() {
    std::cout << "Mot de l'adversaire : ";
    for (char c : opponentMotCache) {
        if (c != '_') {
            std::cout << GREEN << c << RESET << " ";
        } else {
            std::cout << c << " ";
        }
    }
    std::cout << std::endl;
    std::cout << "Tentatives restantes de l'adversaire : " << opponentTentativesRestantes << std::endl;
    std::cout << "Lettres proposées par l'adversaire : ";
    for (char lettre : opponentLettresProposees) {
        std::cout << lettre << " ";
    }
    std::cout << std::endl << std::endl;
}

char Game::demanderLettre() {
    char lettre;
    std::cout << "Proposez une lettre : ";
    std::cin >> lettre;
    return lettre;
}

bool Game::estFinJeu() {
    if (mot->estComplet()) {
        std::cout << GREEN << "Félicitations ! Vous avez deviné votre mot : " << motADeviner << RESET << std::endl;
        motsDevinés++;
        return true;
    }

    if (joueur->getTentativesRestantes() <= 0) {
        std::cout << RED << "Vous avez perdu ! Votre mot était : " << motADeviner << RESET << std::endl;
        return true;
    }
    
    if (opponentMotCache == motADeviner) {
        std::cout << RED << "Votre adversaire a deviné son mot. Vous avez perdu !" << RESET << std::endl;
        return true;
    }

    if (opponentTentativesRestantes <= 0) {
        std::cout << GREEN << "Votre adversaire a épuisé ses tentatives. Vous avez gagné !" << RESET << std::endl;
        return true;
    }

    return false;
}

void Game::envoyerEtatJeu() {
    std::ostringstream oss;
    oss << mot->getMotCache() << "\n";
    oss << joueur->getTentativesRestantes() << "\n";
    for (char lettre : joueur->getLettresProposees()) {
        oss << lettre;
    }
    oss << "\n";

    network->sendGameState(oss.str());
}

void Game::recevoirEtatJeu() {
    std::string data;
    if (network->receiveGameState(data)) {
        std::istringstream iss(data);
        std::getline(iss, opponentMotCache);
        std::string line;
        std::getline(iss, line);
        opponentTentativesRestantes = std::stoi(line);
        std::getline(iss, line);
        opponentLettresProposees.clear();
        for (char c : line) {
            opponentLettresProposees.push_back(c);
        }
    } else {
        std::cerr << RED << "Erreur lors de la réception de l'état du jeu de l'adversaire." << RESET << std::endl;
    }
}

void Game::startGame() {
    chargerStatistiques();
    afficherStatistiques();

    if (network->isServerMode()) {
        envoyerEtatJeu();
    } else {
        recevoirEtatJeu();
    }

    while (true) {
        afficherEtatJeu();

        if (estFinJeu()) {
            network->closeConnection();
            break;
        }

        char lettre = demanderLettre();

        if (!joueur->proposerLettre(lettre)) {
            std::cout << RED << "Lettre déjà proposée, essayez une autre." << RESET << std::endl;
            continue;
        }

        nombreCoups++;

        if (!mot->revelerLettre(lettre)) {
            joueur->decrementerTentatives();
            std::cout << RED << "Mauvaise lettre !" << RESET << std::endl;
        } else {
            std::cout << GREEN << "Bonne lettre !" << RESET << std::endl;
        }

        envoyerEtatJeu();

        recevoirEtatJeu();

        if (estFinJeu()) {
            network->closeConnection();
            break;
        }

    }
    sauvegarderStatistiques();
}

std::string Game::encode(const std::string& data) {
    std::string encoded = data;
    int key = 5; // Clé de décalage pour le chiffrement César
    for (char& c : encoded) {
        c += key;
    }
    return encoded;
}

std::string Game::decode(const std::string& data) {
    std::string decoded = data;
    int key = 5;
    for (char& c : decoded) {
        c -= key;
    }
    return decoded;
}

void Game::chargerStatistiques() {
    std::ifstream fichier(statistiquesPath);
    if (fichier.is_open()) {
        std::string encodedData((std::istreambuf_iterator<char>(fichier)), std::istreambuf_iterator<char>());
        std::string data = decode(encodedData);

        std::istringstream iss(data);
        iss >> nombreCoups >> motsDevinés;

        fichier.close();
    }
}

void Game::sauvegarderStatistiques() {
    std::ofstream fichier(statistiquesPath);
    if (fichier.is_open()) {
        std::string data = std::to_string(nombreCoups) + " " + std::to_string(motsDevinés);
        std::string encodedData = encode(data);
        fichier << encodedData;
        fichier.close();
    }
}

void Game::afficherStatistiques() {
    std::cout << "Vos statistiques :" << std::endl;
    std::cout << "Nombre total de coups : " << nombreCoups << std::endl;
    std::cout << "Nombre de mots devinés avec succès : " << motsDevinés << std::endl << std::endl;
}
