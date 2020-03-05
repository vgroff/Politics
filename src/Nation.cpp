#include "../include/Nation.hpp"
#include "../include/electors/Utility.hpp"
#include "../include/common/Date.hpp"
#include<iostream>

Nation::Nation(std::string name, PopulationProperties populationProperties, Industry privateIndustry)
    : name(name), populationProps(populationProperties), privateIndustry(privateIndustry) {    

}

void Nation::runIndustryTurn() {
    // Increase population size
    growPopulation();
    // Increase employement if possible
    if (atFullEmployement() == false) {
        // TODO: try to increase employement if production capacity is available
    }
    // Calculate wages
    std::map<WorkerType, double> wages = privateIndustry.getWages();
    // Get utilty for each worker type (update the Pops)
    for (std::pair<WorkerType, double> wagePair : wages) {
        double utility = getUtility(wagePair.second);
        std::cout << "Utility for worker " << workerTypeToString(wagePair.first) << " " << utility << ", from wage " << wagePair.second << std::endl;
        // TODO: Update the Pops happiness
    }
    // Calculate profit
    double privateProfit = privateIndustry.getProfit();
    std::cout << "Profit: " << privateProfit << std::endl;
}

void Nation::growPopulation() {
    double growthRatePerTurn = populationProps.yearlyGrowthRate/(12.0/MONTHS_PER_TURN);
    populationProps.population *= (1+growthRatePerTurn);
    std::cout << "Population: " << populationProps.population << std::endl;
}

double Nation::getWorkingPopulation() {
    return populationProps.population*populationProps.workingPopulationRate;
}

bool Nation::atFullEmployement() {
    if (getWorkingPopulation() >= populationProps.baseUnemployementRate*populationProps.population) {
        return true;
    }
    return false;
}

Nation Nation::testSetupSingleNation() {
    struct PopulationProperties props {
        .population = 60,
        .yearlyGrowthRate = 0.009,
        .baseUnemployementRate = 0.04,
        .workingPopulationRate = 0.65,
    };
    Nation nation("United Kingdom",
                  props,
                  Industry::testSetup());
    return nation;
}
