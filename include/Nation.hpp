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
    std::vector<Elector> electors;
    std::map<WorkerEducation, double> workerEducation;
    bool jobsDistributed;
    const double chanceJobRedistributed;
};

class Nation {
private:
    std::string name;
    PopulationProperties populationProps;
    ElectorProperties electorProperties;
    Industry privateIndustry;

    double getWorkingPopulation();
    bool atFullEmployement();

    void distributeJobsToElectors(std::map<WorkerEducation, std::map<WorkerType, double>> jobDist);

    void growPopulation();
    std::map<WorkerEducation, std::map<WorkerType, double>> calculateJobDistribution();
    double distributeJobs(std::map<WorkerEducation, std::map<WorkerType, double>>& actualJobDist,
                          std::map<WorkerType, double> availableJobsDist,
                          std::map<WorkerEducation, double> educationDist);
public:
    Nation(std::string name, PopulationProperties populationProperties, ElectorProperties electorProperties, Industry privateIndustry);
    void runIndustryTurn();

    static Nation testSetupSingleNation();
};

