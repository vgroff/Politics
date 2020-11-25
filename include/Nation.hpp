#pragma once

#include <string>
#include <memory>

#include "Clock.hpp"
#include "common/Modifier.hpp"
#include "common/Operation.hpp"
#include "common/Variable.hpp"
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
    Variable<double> researchVar(0.0);
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

class Nation : ClockSubscriber, std::enable_shared_from_this<Nation> {
    friend class NationWindow;
private:
    std::string name;
    std::weak_ptr<Clock> clock;
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
    void runIndustryTurn();
public:
    Nation(std::weak_ptr<Clock> clock,
           std::string name, 
           PopulationProperties populationProperties, 
           ElectorProperties electorProperties, 
           CapitalistProperties capitalistProperties, 
           ResearchProperties researchProps,
           PoliticalProperties politicalProps,
           Laws laws,
           Industry privateIndustry);
    void init();
    std::string getName();

    static Nation testSetupSingleNation(std::weak_ptr<Clock> clock);
};

