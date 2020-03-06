#include "../../include/electors/Elector.hpp"
#include "../../include/common/Maths.hpp"

std::string workerTypeToString(WorkerType workerType) {
    if (workerType == Unskilled) {
        return "Unskilled";
    } else if (workerType == Skilled) {
        return "Skilled";
    } else if (workerType == HighSkilled) {
        return "High Skilled";
    }
    return "Unknown Worker Type";
}

bool sumsToOne(std::vector<std::pair<WorkerType, double>> dist) {
    double sumDist = 0;
    for (const auto distPair : dist) {
        sumDist += distPair.second;
    }
    if (std::abs(sumDist - 1) < 0.00001) {
        return true;
    }
    return false;
}

void throwIfInconsistent(const std::map<WorkerType, double>& dist1,
                        const std::map<WorkerType, double>& dist2,
                        double sumsToOne1,
                        double sumsToOne2) {
    if (dist1.size() != dist2.size()) {
        throw std::invalid_argument("Pay and worker distribution maps are different sizes");
    }
    std::set<WorkerType> workerTypeSet;
    double sum1 = 0;
    double sum2 = 0;
    for (const auto& distPair : dist1) {
        if (workerTypeSet.find(distPair.first) == workerTypeSet.end()) {
            workerTypeSet.insert(distPair.first);
        } else {
            throw std::invalid_argument("A worker type appears twice");
        } 
        sum1 += distPair.second;
        if (dist2.find(distPair.first) != dist2.end()) {
            sum2 += dist2.at(distPair.first);
        } else {
            throw std::invalid_argument("Mismatch worker distribution maps");
        }
    }
    if ((std::abs(sum1 - 1.0) > 0.0001 && sumsToOne1) || (std::abs(sum2 - 1.0) > 0.0001 && sumsToOne2)) {
        throw std::invalid_argument("Ratios don't sum to 1");
    }
}

Elector::Elector(WorkerType workerType, double longTermtility) 
    : longTermUtility(longTermUtility), shortTermUtility(shortTermUtility), currentUtility(currentUtility), workerType(workerType) {

};

double Elector::setUtility(double newUtility) {
    currentUtility = newUtility;
    shortTermUtility = weightedMovingAvg(shortTermMultiplier, shortTermUtility, newUtility);
    longTermUtility  = weightedMovingAvg(longTermMultiplier, longTermUtility, newUtility);
}

double Elector::getLongTermUtility() {
    return longTermUtility;
}

double Elector::getShortTermUtility() {
    return shortTermUtility;
}

WorkerType Elector::getWorkerType() {
    return workerType;
}

bool Elector::canWorkJob(WorkerType job) {
    if (workerEducation >= HighSchool && job <= Skilled) {
        return true;
    } if (workerEducation >= University && job <= HighSkilled) {
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<Elector>> Elector::generateTestElectors(int numElectors, 
                                                    std::map<WorkerEducation, double> distribution, 
                                                    std::map<WorkerEducation, double> utilityDistribution) {

    throwIfInconsistent(distribution, utilityDistribution, true, false);
    std::vector<std::shared_ptr<Elector>> electors;
    for (int i = 0; i < numElectors; i++) {
        double coinFlip = (double)std::rand()/RAND_MAX;
        double currentSum = 0;
        for (const auto distPair : distribution) {
            currentSum += distPair.second;
            if (currentSum > coinFlip) {
                std::shared_ptr<Elector> e(new Elector(distPair.first, utilityDistribution.at(distPair.first)));
                electors.push_back(e);
                break;
            }
        }
    }
    return electors;
}