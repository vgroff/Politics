#include "../../include/electors/Elector.hpp"
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


Elector::Elector(WorkerEducation workerEducation, double longTermUtility, PoliticalCompassPoint baseIdeology) 
    : longTermUtility(longTermUtility), 
    shortTermUtility(longTermUtility), 
    currentUtility(longTermUtility),
    workerEducation(workerEducation),
    baseIdeology(baseIdeology) {

};

std::string Elector::toString() {
    std::ostringstream strStream;
    strStream << "Ideology: " << baseIdeology.toString() << std::endl;
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