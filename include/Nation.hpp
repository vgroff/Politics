#pragma once

#include <string>

#include "Economy/Industry.hpp"

class Nation {
    std::string name;
    double population;
    Industry privateIndustry;
public:
    Nation(std::string name, double population, Industry privateIndustry);
    void runIndustryTurn();

    static Nation testSetupSingleNation();
};
