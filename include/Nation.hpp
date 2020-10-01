#pragma once

#include <string>

#include "economy/Industry.hpp"
#include "electors/Elector.hpp"
#include "politics/PoliticalCompassPointGenerator.hpp"
#include "politics/Party.hpp"
#include "politics/Laws.hpp"

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

struct Laws {
    BudgetLaws budgetLaws;
    LabourLaws labourLaws;
    CriminalLaws criminalLaws;
};
// Have Bill class that (see notez)

class Nation {
    friend class NationWindow;
private:
    std::string name;
    PopulationProperties populationProps;
    ElectorProperties electorProps;
    CapitalistProperties capitalistProps;
    ResearchProperties researchProps;
    PoliticalProperties politicalProps;
    Laws laws;
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
    void capitalistsInvest();
    void politicsTurn();
public:
    Nation(std::string name, 
           PopulationProperties populationProperties, 
           ElectorProperties electorProperties, 
           CapitalistProperties capitalistProperties, 
           ResearchProperties researchProps,
           PoliticalProperties politicalProps,
           Laws laws,
           Industry privateIndustry);
    void runIndustryTurn();
    std::string getName();

    static Nation testSetupSingleNation();
};

