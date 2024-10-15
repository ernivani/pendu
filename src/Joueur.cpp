#include "Joueur.h"

using namespace std;

Joueur::Joueur(int tentativesMax) : tentativesRestantes(tentativesMax) {}

bool Joueur::proposerLettre(char lettre) {
    for (char l : lettresProposees) {
        if (l == lettre) {
            return false;
        }
    }
    lettresProposees.push_back(lettre);
    return true;
}

int Joueur::getTentativesRestantes() const {
    return tentativesRestantes;
}

const std::vector<char>& Joueur::getLettresProposees() const {
    return lettresProposees;
}

void Joueur::decrementerTentatives() {
    tentativesRestantes--;
}
