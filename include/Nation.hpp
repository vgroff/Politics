#pragma once

#include <string>

#include "economy/Industry.hpp"
#include "electors/Elector.hpp"
#include "politics/PoliticalCompassPointGenerator.hpp"
#include "politics/Party.hpp"

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

struct CapitalistProperties {
    double numCapitalists;
    const double minUtility;
};

struct ResearchProperties {
    double research;
};

struct PoliticalProperties {
    std::vector<Party> parties;
};

class Nation {
private:
    std::string name;
    PopulationProperties populationProps;
    ElectorProperties electorProps;
    CapitalistProperties capitalistProps;
    ResearchProperties researchProps;
    PoliticalProperties politicalProps;
    Industry privateIndustry;
    double capitalInTheBank = 0;

    double getWorkingPopulation();
    double getEmployablePopulation();
    bool atFullEmployement();

    void distributeJobsToElectors(std::map<WorkerEducation, std::map<WorkerType, double>> jobDist);

    double getProductionCapacityInvestement(double investement);
    double getProductivityInvestement(double investement);

    void growPopulation();
    std::map<WorkerEducation, std::map<WorkerType, double>> calculateJobDistribution();
    double distributeJobs(std::map<WorkerEducation, std::map<WorkerType, double>>& actualJobDist,
                          std::map<WorkerType, double> availableJobsDist,
                          std::map<WorkerEducation, double> educationDist);
    void capitalistsInvest();
    void politicsTurn();
public:
    Nation(std::string name, 
           PopulationProperties populationProperties, 
           ElectorProperties electorProperties, 
           CapitalistProperties capitalistProperties, 
           ResearchProperties researchProps,
           PoliticalProperties politicalProps,
           Industry privateIndustry);
    void runIndustryTurn();

    static Nation testSetupSingleNation();
};

