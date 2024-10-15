#include "Mot.h"

Mot::Mot(const std::string& mot) : mot(mot), motCache(mot.size(), '_') {}

Mot::Mot(const std::string& motADeviner, const std::string& motCache) : mot(motADeviner), motCache(motCache) {}

std::string Mot::getMotCache() const {
    return motCache;
}

bool Mot::revelerLettre(char lettre) {
    bool lettreTrouvee = false;
    for (size_t i = 0; i < mot.size(); ++i) {
        if (mot[i] == lettre) {
            motCache[i] = lettre;
            lettreTrouvee = true;
        }
    }
    if (lettreTrouvee) {
        lettresDevinees.push_back(lettre);
    }
    return lettreTrouvee;
}

bool Mot::estComplet() const {
    return motCache == mot;
}
