#pragma once
#include <vector>
#include <string>

class Mot {
private:
    std::string mot;
    std::string motCache;
    std::vector<char> lettresDevinees;
public:
    Mot(const std::string& mot);
    Mot(const std::string& motADeviner, const std::string& motCache);
    std::string getMotCache() const;
    bool revelerLettre(char lettre);
    bool estComplet() const;
};