#pragma once
#include <vector>



class Joueur {
private:
    int tentativesRestantes;
    std::vector<char> lettresProposees;
public:
    Joueur(int tentativesMax);
    bool proposerLettre(char lettre);
    int getTentativesRestantes() const;
    const std::vector<char>& getLettresProposees() const;
    void decrementerTentatives();
};