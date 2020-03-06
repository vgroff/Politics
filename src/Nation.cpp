#include "../include/Nation.hpp"
#include "../include/electors/Utility.hpp"
#include "../include/common/Date.hpp"
#include "../include/common/Maths.hpp"
#include<iostream>

Nation::Nation(std::string name, PopulationProperties populationProperties,  ElectorProperties electorProperties, Industry privateIndustry)
    : name(name), populationProps(populationProperties), electorProperties(electorProperties), privateIndustry(privateIndustry) {    

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
    // Bind electors to their workplaces

    // Get utilty for each worker type (update the Pops)
    std::cout << "Utilities: " << std::endl;
    for (std::pair<WorkerType, double> wagePair : wages) {
        double utility = getUtility(wagePair.second);
        for (auto& elector : electorProperties.electors) {
            if (wagePair.first == elector->getWorkerType()) {
                elector->setUtility(utility);
                std::cout << workerTypeToString(elector->getWorkerType()) << "(" << utility << ", " << elector->getLongTermUtility() << ", " << elector->getShortTermUtility() << "), " << std::endl;
            }
        }
    }
    std::cout << std::endl;
    // Calculate profit
    double privateProfit = privateIndustry.getProfit();
    std::cout << "Profit: " << privateProfit << std::endl;
}

void Nation::distributeJobsToElectors() {
    std::map<WorkerType, double> workerDist = privateIndustry.getWorkerDistribution();
    // - Algorithm should start with the most well educated people and fill in as many jobs as possible!
    // Remember to apply the unemployement rate beforehand
}

void Nation::growPopulation() {
    double scalingFactor = 1/(12.0/MONTHS_PER_TURN);
    double growthRatePerTurn = std::pow(1+populationProps.yearlyGrowthRate, scalingFactor);
    populationProps.population *= growthRatePerTurn;
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
    PopulationProperties props {
        .population = 60,
        .yearlyGrowthRate = 0.009,
        .baseUnemployementRate = 0.04,
        .workingPopulationRate = 0.65,
    };
    auto electorsVec = Elector::generateTestElectors(15, 
                                                {{HighSkilled, 0.25}, {Skilled, 0.5}, {Unskilled, 0.25}},
                                                {{HighSkilled, 0.7}, {Skilled, 0.5}, {Unskilled, 0.3}});
    ElectorProperties electors {
        .electors = electorsVec,
    };

    Nation nation("United Kingdom",
                  props,
                  electors,
                  Industry::testSetup());
    return nation;
}
