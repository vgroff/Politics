#pragma once

#include <string>

#include "economy/Industry.hpp"
#include "electors/Elector.hpp"

struct PopulationProperties {
    double population;
    double yearlyGrowthRate;
    double baseUnemployementRate;
    double workingPopulationRate;
};

class Nation {
private:
    std::string name;
    PopulationProperties populationProps;
    Industry privateIndustry;

    void growPopulation();
    double getWorkingPopulation();
    bool atFullEmployement();
public:
    Nation(std::string name, PopulationProperties populationProperties, Industry privateIndustry);
    void runIndustryTurn();

    static Nation testSetupSingleNation();
};

