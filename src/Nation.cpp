#include "../include/Nation.hpp"
#include "../include/electors/Utility.hpp"
#include "../include/common/Date.hpp"
#include "../include/common/Maths.hpp"
#include "../include/politics/PoliticalCompassPointGenerator.hpp"
#include "../include/electors/Utility.hpp"

#include<iostream>
#include<memory>

// Distribute available jobs amongst educations. Actual job dist is the output, along with the minimum ratio of jobs filled (due to lack of education)
double distributeJobs(std::map<WorkerEducation, std::map<WorkerType, double>>& actualJobDist,
                      std::map<WorkerType, double> availableJobsDist,
                      std::map<WorkerEducation, double> educationDist) {
    WorkerEducation currentEducation = BEST_EDUCATION;
    std::map<WorkerType, double> jobsFilled;
    double minRatio = 1;
    bool outOfWorkers = false;
    bool outOfJobs = false;
    for (auto it = availableJobsDist.rbegin(); it != availableJobsDist.rend(); it++) {
        // Distribute jobs in order of skill
        WorkerType job = it->first;
        double numJobs = it->second;
        while (true) {
            if (Elector::canWorkJob(currentEducation, job)) {
                double numWorkersLeft = educationDist.at(currentEducation);
                if (numJobs > numWorkersLeft) {
                    actualJobDist[currentEducation][job] += numWorkersLeft;
                    educationDist[currentEducation] -= numWorkersLeft;
                    jobsFilled[job] += numWorkersLeft;
                    numJobs -= numWorkersLeft;
                } else if (numJobs <= numWorkersLeft) {
                    actualJobDist[currentEducation][job] += numJobs;
                    educationDist[currentEducation] -= numJobs;
                    jobsFilled[job] += numJobs;
                    break;
                }
            } else {
                outOfWorkers = true;
                double ratio = jobsFilled[job] / availableJobsDist[job];
                if (ratio < minRatio) {
                    minRatio = ratio;
                }
                break;
            }

            if (currentEducation == WORST_EDUCATION) {
                break;
            }
            currentEducation = (WorkerEducation) (currentEducation - 1);
        }
    }
    return minRatio;
}

Nation::Nation(std::weak_ptr<Clock> clock,
               std::string name, 
               PopulationProperties populationProperties,
               ElectorProperties electorProps, 
               CapitalistProperties capitalistProps, 
               ResearchProperties researchProps,
               PoliticalProperties politicalProps,
               Laws laws,
               Industry privateIndustry)
    : name(name), 
    ClockSubscriber(clock),
    populationProps(populationProperties), 
    electorProps(electorProps), 
    capitalistProps(capitalistProps),
    researchProps(researchProps),
    politicalProps(politicalProps),
    laws(laws),
    privateIndustry(privateIndustry)
    {
    if (sumsToOne(electorProps.workerEducation) == false) {
        throw std::invalid_argument("Worker education does not sum to one");
    }
}

std::string Nation::getName() {
    return name;
}

void Nation::init() {
    subscribeToClock([this](time_point currentDate, size_t num) {
        this->runIndustryTurn();
    });
}

void Nation::runIndustryTurn() {
    std::cout << "Research level: " << researchProps.research << std::endl;
    double productivityDiff = (researchProps.research - privateIndustry.getProductivity())/researchProps.research;
    researchProps.research = researchProps.research*(1 + 0.01*MONTHS_PER_TURN/12 + 0.015*std::pow(productivityDiff, 2)*MONTHS_PER_TURN/12);
    privateIndustry.setCurrentTechnology(researchProps.research);

    std::weak_ptr<Variable<double>> researchVar(researchProps.researchVar);
    std::function<std::shared_ptr<Addition<double>>(Value<double>)> getModif = [](Value<double> researchVar) {
        auto addition = Value<double>(std::string("increase in research"), 0.1);
        return std::make_shared<Addition<double>>(researchVar, addition); // This bit should be done by the modifier, or it needs to take the current value too
    };
    auto modifier = std::make_shared<AdditiveModifier<double>>(std::string("Modifier for research"), getModif);
    researchProps.researchVar->addModifier(modifier);
    researchProps.researchVar->calculateValue(true);
    std::cout << researchProps.researchVar->getLatest().getValue();
    throw std::exception();

    // Increase population size
    growPopulation();
    // Increase employement if possible
    if (atFullEmployement() == false) {
        // TODO: try to increase employement if production capacity is available
    }
    std::map<WorkerEducation, std::map<WorkerType, double>> jobDist = calculateJobDistribution();
    distributeJobsToElectors(jobDist);

    // Calculate wages
    std::map<WorkerType, double> wages = privateIndustry.getWages();

    // Get utilty for each worker type (update the Pops)
    std::cout << "Utilities: " << std::endl;
    for (std::pair<WorkerType, double> wagePair : wages) {
        double utility = getUtility(wagePair.second);
        for (auto& elector : electorProps.electors) {
            if (wagePair.first == elector.getWorkerType()) {
                elector.setUtility(utility);
            }
        }
    }
    std::cout << std::endl;
    capitalistsInvest();
    politicsTurn();
}

void Nation::politicsTurn() {
    std::vector<double> voteShare(politicalProps.parties.size());
    double blankVotes = 0;
    for (auto elector : electorProps.electors) {
        std::cout << elector.toString();
        auto result = elector.vote(politicalProps.parties);
        std::cout << "Voted for: ";
        if (result == nullptr) {
            std::cout << "Nobody" << std::endl;
            blankVotes += 1;
        } else {
            std::cout << politicalProps.parties[*result].getName() << std::endl;
            voteShare[*result] += 1;
        }
        std::cout << std::endl;
    }
    double sizeOfElectorate = electorProps.electors.size();
    std::cout << "Vote Share: " << std::endl;
    std::cout << "Blank: " << blankVotes/sizeOfElectorate << std::endl;
    for (size_t i = 0; i < politicalProps.parties.size(); i++) {
        const Party& party = politicalProps.parties[i];
        std::cout << party.getName() << ": " << voteShare[i]/sizeOfElectorate << std::endl;
    }
}

void Nation::capitalistsInvest() {
    // Calculate profit
    double privateProfit = privateIndustry.getProfit();
    std::cout << "Profit: " << privateProfit << std::endl;
    double costOfLiving = capitalistProps.numCapitalists*inverseUtility(capitalistProps.minUtility);
    if (costOfLiving < privateProfit) {
        double investement = privateProfit - costOfLiving;
        double newProfit1 = getProductivityInvestement(investement);
        double newProfit2 = getProductionCapacityInvestement(investement);
        std::cout << newProfit1 << " " << newProfit2 << std::endl;

        double changeInProfit1 = (newProfit1 - privateProfit) - investement/(12*10/MONTHS_PER_TURN);
        double changeInProfit2 = (newProfit2 - privateProfit) - investement/(12*10/MONTHS_PER_TURN);
        std::cout << "Changes in profit " << changeInProfit1 << " " << changeInProfit2 << std::endl;
        if (changeInProfit1 > 0 || changeInProfit2 > 0) {
            if (newProfit1 > newProfit2) {
                privateIndustry.makeProductivityInvestement(investement);
                std::cout << "Invested " << investement << " in productivity (now " << privateIndustry.getProductivity() << ")" << std::endl; 
            } else {
                privateIndustry.makeProductionCapacityInvestement(investement);
                std::cout << "Invested " << investement << " in production capacity (now " << privateIndustry.getProductionCapacity() << ")" << std::endl; 
            }
        } else {
            capitalInTheBank += investement;
            std::cout << "No investement, capital in the bank: " << capitalInTheBank << std::endl;
        }
        privateIndustry.printStatus();
        const double capitalRatioConsumed = 0.2;
        double finalCapitalistIncome = costOfLiving/capitalistProps.numCapitalists + capitalInTheBank*capitalRatioConsumed/capitalistProps.numCapitalists;
        capitalInTheBank -= capitalInTheBank*capitalRatioConsumed;
        double capitalistUtility = getUtility(finalCapitalistIncome);
        std::cout << "Capitalist Utility: " << capitalistUtility << " Cost: " << finalCapitalistIncome << std::endl;
    }
}

double Nation::getProductionCapacityInvestement(double investement) {
    Industry i2 = privateIndustry.theoreticalProductionCapacityInvestement(investement);
    double newProfit2 = 0;
    bool industryFullyEmployed = 1.00001*privateIndustry.getNumWorkers() >= privateIndustry.getProductionCapacity(); 
    bool populationFullyEmployed = 1.00001*privateIndustry.getNumWorkers() >= getEmployablePopulation();
    if (industryFullyEmployed && !populationFullyEmployed) {
        // Don't invest if all jobs are not filled
        // Don't invest if there arent workers to fill the jobs
        i2.setNumWorkers(i2.getProductionCapacity());
        newProfit2 = i2.getProfit();
    }
    return newProfit2;
}

double Nation::getProductivityInvestement(double investement) {
    Industry i = privateIndustry.theoreticalProductivityInvestement(investement);
    return i.getProfit();
}

void Nation::distributeJobsToElectors(std::map<WorkerEducation, std::map<WorkerType, double>> jobDist) {
    double chanceJobRedistributed = 0.2;
    if (electorProps.jobsDistributed == true) {
        chanceJobRedistributed = electorProps.chanceJobRedistributed;
    } else {
        electorProps.jobsDistributed = true;
    }
    for (size_t i = 0; i < electorProps.electors.size(); i++) {
        if (coinFlip(chanceJobRedistributed)) {
            auto jobDistPair = jobDist.find(electorProps.electors[i].getWorkerEducation());
            if (jobDistPair != jobDist.end()) {
                auto job = coinFlip(jobDistPair->second);
                electorProps.electors[i].setWorkerType(job);
                std::cout << "Job is now " << workerTypeToString(job) << std::endl;;
            } else {
                electorProps.electors[i].setWorkerType(Unemployed);
            }
        }
    }
}

std::map<WorkerEducation, std::map<WorkerType, double>> Nation::calculateJobDistribution() {
    std::map<WorkerType, double> theoreticalJobDist = privateIndustry.getWorkerDistribution();
    std::map<WorkerEducation, double> educationDist = electorProps.workerEducation;
    double workingRate = populationProps.workingPopulationRate;
    double workingPopulation = getWorkingPopulation();
    double baseUnemployement = populationProps.baseUnemployementRate;

    if (privateIndustry.getNumJobs() / workingPopulation < 1) {
        // If we dont have enough jobs for everyone we need to scale down the calculation
        for (auto& jobPair : theoreticalJobDist) {
            // Modify by population size
            jobPair.second *= privateIndustry.getNumJobs() / workingPopulation;
        }
    } // If there are too many jobs, that will be dealt with by the minium ratio
    
    for (auto& educationPair : educationDist) {
        // Apply unemployement effect (applied flatly to all educations)
        // TODO: Make this more intelligent
        educationPair.second = (1 - baseUnemployement)*educationPair.second;
    }
    std::map<WorkerEducation, std::map<WorkerType, double>> testJobDist = {{WORST_EDUCATION, {}}};
    WorkerEducation currentEducation = WORST_EDUCATION;
    while (currentEducation < BEST_EDUCATION) {
        currentEducation = (WorkerEducation) (currentEducation + 1);
        testJobDist[currentEducation] = {};
    }
    double minJobFilledRatio = distributeJobs(testJobDist, theoreticalJobDist, educationDist);
    // Set the amount of workers
    double employablePopulation = getEmployablePopulation();
    if ( employablePopulation / privateIndustry.getNumJobs() < 1) {
        privateIndustry.setNumWorkers(employablePopulation);
    } else {
        privateIndustry.setNumWorkers(minJobFilledRatio*privateIndustry.getNumJobs());
    }
    std::map<WorkerEducation, std::map<WorkerType, double>> actualJobDist;
    if (minJobFilledRatio < 0.99) {
        WorkerEducation currentEducation = WORST_EDUCATION;
        while (currentEducation < BEST_EDUCATION) {
            currentEducation = (WorkerEducation) (currentEducation + 1);
            actualJobDist[currentEducation] = {};
        }
        // If not all jobs were filled, we need to decrease to job number
        for (auto& jobPair : theoreticalJobDist) {
            // Modify by population size
            jobPair.second *= minJobFilledRatio;
        }
        minJobFilledRatio = distributeJobs(actualJobDist, theoreticalJobDist, educationDist);
    } else {
        actualJobDist = testJobDist;
    }
    if (std::abs(minJobFilledRatio - 1) > 0.001) {
        throw std::logic_error("Min ratio of jobs filled is not 1");
    }

    // Normalise the values in workerDist to get a probability distribution out
    for (auto& workerJobDist : actualJobDist) {
        double sum = 0;
        for (auto& workerPair: workerJobDist.second) {
            sum += workerPair.second;
        }
        double diff = electorProps.workerEducation.at(workerJobDist.first) - sum;
        if (diff > 0.0001) {
            // If there are workers left over, the rest are unemployed
            workerJobDist.second[Unemployed] += diff;
        } else if (diff < 0.0001) {
            std::logic_error("More jobs given to workers than workers actually exist");
        }
        for (auto& workerPair: workerJobDist.second) {
            workerPair.second = workerPair.second / electorProps.workerEducation.at(workerJobDist.first);
        }
    }
    return actualJobDist;
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

double Nation::getEmployablePopulation() {
    return populationProps.population*populationProps.workingPopulationRate*(1-populationProps.baseUnemployementRate);
}

bool Nation::atFullEmployement() {
    if (getWorkingPopulation() >= populationProps.baseUnemployementRate*populationProps.population) {
        return true;
    }
    return false;
}

Nation Nation::testSetupSingleNation(std::weak_ptr<Clock>  clock) {
    PopulationProperties props {
        .population = 60,
        .yearlyGrowthRate = 0.009,
        .baseUnemployementRate = 0.04,
        .workingPopulationRate = 0.65,
    };
    std::map<WorkerEducation, double> workerEducationMap = {{University, 0.25}, {HighSchool, 0.5}, {School, 0.25}};
    auto electorsVec = Elector::generateTestElectors(200, 
                                                workerEducationMap,
                                                {{University, 0.7}, {HighSchool, 0.5}, {School, 0.3}},
                                                PoliticalCompassPointGenerator::testSetup());
    ElectorProperties electors {
        .electors = electorsVec,
        .workerEducation = workerEducationMap,
        .jobsDistributed = false,
        .chanceJobRedistributed = 0.15*MONTHS_PER_TURN,
    };
    CapitalistProperties capitalistProps {
        .numCapitalists = 0.3,
        .minUtility = 0.85
    };
    ResearchProperties researchProps {
        .research = 1.06
    };
    PoliticalCompassPoint labourIdeology({{Capitalist, -0.28}, {Conservative, -0.32}, {Nationalist, -0.4}});
    PoliticalCompassPoint libDemIdeology({{Capitalist, 0.15}, {Conservative, -0.15}, {Nationalist, -0.6}});
    PoliticalCompassPoint toryIdeology({{Capitalist, 0.45}, {Conservative, 0.35}, {Nationalist, 0.45}});
    PoliticalProperties politicalProps {
        .parties = {Party("Labour", labourIdeology), Party("Liberal Democrats", libDemIdeology), Party("Conservative", toryIdeology)}
    };

    BudgetLaws budgetLaws(flatConsumptionTax(0.0),
                            {},
                            0,
                            0);
    Laws laws = {
        .budgetLaws = budgetLaws
    };

    Nation nation(clock,
                  "United Kingdom",
                  props,
                  electors,
                  capitalistProps,
                  researchProps,
                  politicalProps,
                  laws,
                  Industry::testSetup());
    return nation;
}
