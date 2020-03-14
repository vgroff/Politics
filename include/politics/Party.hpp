#pragma once
#include<string>
#include "PoliticalCompassPoint.hpp"

class Party {
private:
    std::string name;
    PoliticalCompassPoint currentIdeology;
public:
    Party(std::string name, PoliticalCompassPoint ideology);
    PoliticalCompassPoint getCurrentIdeology() const;
    std::string getName() const;
};
