#include "../../include/electors/Elector.hpp"
#include "../../include/politics/Party.hpp"
#include "../../include/common/Maths.hpp"
#include<iostream>
#include<sstream>

std::string workerTypeToString(WorkerType workerType) {
    if (workerType == Unemployed) {
        return "Unemployed";
    } else if (workerType == Unskilled) {
        return "Unskilled";
    } else if (workerType == Skilled) {
        return "Skilled";
    } else if (workerType == HighSkilled) {
        return "High Skilled";
    }
    return "Unknown Worker Type";
}

std::string workerEducationToString(WorkerEducation workerEducation) {
    if (workerEducation == School) {
        return "School";
    } else if (workerEducation == HighSchool) {
        return "High School";
    } else if (workerEducation == University) {
        return "University";
    }
    return "Unknown Worker Education";
}


Elector::Elector(WorkerEducation workerEducation, double longTermUtility, PoliticalCompassPoint currentIdeology) 
    : longTermUtility(longTermUtility), 
    shortTermUtility(longTermUtility), 
    currentUtility(longTermUtility),
    workerEducation(workerEducation),
    currentIdeology(currentIdeology) {

};

// TODO: Overload this function with voting for an MP
// TODO: Overload this function for tactical voting (feed in polls)
std::shared_ptr<size_t> Elector::vote(const std::vector<Party>& choices) {
    const double min = 0.14;
    double bestScore = min + 0.1;
    std::shared_ptr<size_t> result = nullptr;
    for (size_t i = 0; i < choices.size(); i++) {
        Party party = choices[i];
        double score = currentIdeology.getDistanceTo(party.getCurrentIdeology(), PoliticalCompassPoint::getDefaultStrengths());
        std::cout << score << std::endl;
        if (score < bestScore) {
            bestScore = score;
            result = std::make_shared<size_t>(i);
        }
    }
    if (bestScore > min) {
        return nullptr;
    }
    return result;
}

std::string Elector::toString() {
    std::ostringstream strStream;
    strStream << "Ideology: " << currentIdeology.toString() << std::endl;
    strStream << "Utility: " << currentUtility << ", " << shortTermUtility << ", " << longTermUtility << std::endl;
    return strStream.str();
}

double Elector::setUtility(double newUtility) {
    currentUtility = newUtility;
    shortTermUtility = weightedMovingAvg(1 - shortTermMultiplier, shortTermUtility, newUtility);
    longTermUtility  = weightedMovingAvg(1 - longTermMultiplier, longTermUtility, newUtility);
}

double Elector::getLongTermUtility() {
    return longTermUtility;
}

double Elector::getShortTermUtility() {
    return shortTermUtility;
}

WorkerEducation Elector::getWorkerEducation() {
    return workerEducation;
}

WorkerType Elector::getWorkerType() {
    return workerType;
}

void Elector::setWorkerType(WorkerType newWorkerType) {
    workerType = newWorkerType;
}

bool Elector::canWorkJob(WorkerType job) {
    return Elector::canWorkJob(workerEducation, job);
}

bool Elector::canWorkJob(WorkerEducation workerEducation, WorkerType job) {
    if (workerEducation >= HighSchool && job <= Skilled) {
        return true;
    } else if (workerEducation >= University && job <= HighSkilled) {
        return true;
    } else if (workerEducation == School && job == Unskilled) {
        return true;
    }
    return false;
}

std::vector<Elector> Elector::generateTestElectors(int numElectors, 
                                                    std::map<WorkerEducation, double> distribution, 
                                                    std::map<WorkerEducation, double> utilityDistribution,
                                                    PoliticalCompassPointGenerator generator) {

    throwIfInconsistent(distribution, utilityDistribution, true, false);
    std::vector<Elector> electors;
    for (int i = 0; i < numElectors; i++) {
        WorkerEducation education = coinFlip(distribution);
        Elector e(education, utilityDistribution.at(education), generator.sample());
        std::cout << "Generated elector: " << std::endl << e.toString();
        electors.push_back(e);
    }
    return electors;
}