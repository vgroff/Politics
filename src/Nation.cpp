#include "../include/Nation.hpp"
#include "../include/electors/Utility.hpp"
#include "../include/common/Date.hpp"
#include "../include/common/Maths.hpp"
#include<iostream>

Nation::Nation(std::string name, 
               PopulationProperties populationProperties,
               ElectorProperties electorProperties, 
               CapitalistProperties capitalistProps, 
               Industry privateIndustry)
    : name(name), 
    populationProps(populationProperties), 
    electorProperties(electorProperties), 
    capitalistProps(capitalistProps),
    privateIndustry(privateIndustry) {    
    if (sumsToOne(electorProperties.workerEducation) == false) {
        throw std::invalid_argument("Worker education does not sum to one");
    }
}

void Nation::runIndustryTurn() {
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
        for (auto& elector : electorProperties.electors) {
            if (wagePair.first == elector.getWorkerType()) {
                elector.setUtility(utility);
                std::cout << workerTypeToString(elector.getWorkerType()) << " " << workerEducationToString(elector.getWorkerEducation()) << " " << wagePair.second
                << ", " << "(" << utility << ", " << elector.getLongTermUtility() << ", " << elector.getShortTermUtility() << "), " << std::endl;
            }
        }
    }
    std::cout << std::endl;
    // Calculate profit
    double privateProfit = privateIndustry.getProfit();
    std::cout << "Profit: " << privateProfit << std::endl;
    double costOfLiving = capitalistProps.numCapitalists*inverseUtility(capitalistProps.minUtility);
    std::cout << "CostOfLiving: " << costOfLiving << std::endl;
    if (costOfLiving < privateProfit) {
        double investement = privateProfit - costOfLiving;
        Industry i = privateIndustry.theoreticalProductivityInvestement(investement);
        Industry i2 = privateIndustry.theoreticalProductionCapacityInvestement(investement);

        double newProfit1 = i.getProfit();
        double newProfit2 = 0;
        bool industryFullyEmployed = 1.00001*privateIndustry.getNumWorkers() => privateIndustry.getProductionCapacity(); 
        bool populationFullyEmployed = 1.00001*privateIndustry.getNumWorkers() => getEmployablePopulation();
        if (industryFullyEmployed && populationFullyEmployed) {
            i2.setNumWorkers(i2.getProductionCapacity());
            newProfit2 = i2.getProfit();
        }
        double changeInProfit1 = newProfit1 - privateProfit;
        double changeInProfit2 = newProfit2 - privateProfit;
        if (changeInProfit1 > 0 || changeInProfit2 > 0) {
            if (newProfit1 > newProfit2) {

            } else {

            }
        }

        // Check if num workers is equal to production capacity before re-investing
    }
}

void Nation::distributeJobsToElectors(std::map<WorkerEducation, std::map<WorkerType, double>> jobDist) {
    double chanceJobRedistributed = 1;
    if (electorProperties.jobsDistributed == true) {
        chanceJobRedistributed = electorProperties.chanceJobRedistributed;
    } else {
        electorProperties.jobsDistributed = true;
    }
    for (size_t i = 0; i < electorProperties.electors.size(); i++) {
        if (coinFlip(chanceJobRedistributed)) {
            auto jobDistPair = jobDist.find(electorProperties.electors[i].getWorkerEducation());
            if (jobDistPair != jobDist.end()) {
                auto job = coinFlip(jobDistPair->second);
                electorProperties.electors[i].setWorkerType(job);
                std::cout << "Job is now " << workerTypeToString(job) << std::endl;;
            } else {
                electorProperties.electors[i].setWorkerType(Unemployed);
            }
        }
    }
}

std::map<WorkerEducation, std::map<WorkerType, double>> Nation::calculateJobDistribution() {
    std::map<WorkerType, double> theoreticalJobDist = privateIndustry.getWorkerDistribution();
    std::map<WorkerEducation, double> educationDist = electorProperties.workerEducation;
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
    if (workingPopulation / privateIndustry.getNumJobs() < 1) {
        privateIndustry.setNumWorkers(workingPopulation);
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
        double diff = electorProperties.workerEducation.at(workerJobDist.first) - sum;
        if (diff > 0.0001) {
            // If there are workers left over, the rest are unemployed
            workerJobDist.second[Unemployed] += diff;
        } else if (diff < 0.0001) {
            std::logic_error("More jobs given to workers than workers actually exist");
        }
        for (auto& workerPair: workerJobDist.second) {
            workerPair.second = workerPair.second / electorProperties.workerEducation.at(workerJobDist.first);
        }
    }
    return actualJobDist;
}

// Distribute available jobs amongst educations. Actual job dist is the output, along with the minimum ratio of jobs filled (due to lack of education)
double Nation::distributeJobs(std::map<WorkerEducation, std::map<WorkerType, double>>& actualJobDist,
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

Nation Nation::testSetupSingleNation() {
    PopulationProperties props {
        .population = 60,
        .yearlyGrowthRate = 0.009,
        .baseUnemployementRate = 0.04,
        .workingPopulationRate = 0.65,
    };
    std::map<WorkerEducation, double> workerEducationMap = {{University, 0.25}, {HighSchool, 0.5}, {School, 0.25}};
    auto electorsVec = Elector::generateTestElectors(15, 
                                                workerEducationMap,
                                                {{University, 0.7}, {HighSchool, 0.5}, {School, 0.3}});
    ElectorProperties electors {
        .electors = electorsVec,
        .workerEducation = workerEducationMap,
        .jobsDistributed = false,
        .chanceJobRedistributed = 0.15*MONTHS_PER_TURN,
    };
    CapitalistProperties capitalistProps {
        .numCapitalists = 0.3,
        .minUtility = 0.8
    };

    Nation nation("United Kingdom",
                  props,
                  electors,
                  capitalistProps,
                  Industry::testSetup());
    return nation;
}
