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

struct ElectorProperties {
    std::vector<std::shared_ptr<Elector>> electors;
};

class Nation {
private:
    std::string name;
    PopulationProperties populationProps;
    ElectorProperties electorProperties;
    Industry privateIndustry;

    double getWorkingPopulation();
    bool atFullEmployement();

    void growPopulation();
    void distributeJobsToElectors();
public:
    Nation(std::string name, PopulationProperties populationProperties, ElectorProperties electorProperties, Industry privateIndustry);
    void runIndustryTurn();

    static Nation testSetupSingleNation();
};

