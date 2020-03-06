#pragma once

#include<string>
#include"../common/Date.hpp"

enum WorkerType {
    Unskilled = 0,
    Skilled,
    HighSkilled,
    Capitalist // Forgive me
};

enum WorkerEducation {
    School = 0,
    HighSchool,
    University
};

std::string workerTypeToString(WorkerType workerType);
void throwIfInconsistent(const std::map<WorkerType, double>& workerDistribution,
                        const std::map<WorkerType, double>& payDistribution,
                        double sumsToOne1 = true,
                        double sumToOne2 = true);

class Elector {
private:
    const double shortTermMultiplier = 0.35*MONTHS_PER_TURN; // Might not scale well (not linear)
    const double longTermMultiplier  = 0.05*MONTHS_PER_TURN;
    double currentUtility;
    double shortTermUtility;
    double longTermUtility;
    WorkerType workerType;
    WorkerEducation workerEducation;
public:
    double setUtility(double newUtility);
    double getCurrentUtility();
    double getShortTermUtility();
    double getLongTermUtility();
    WorkerType getWorkerType();
    bool canWorkJob(WorkerType jobType);
    Elector(WorkerType workerType, WorkerEducation workerEducation, double longTermUtility);

    static std::vector<std::shared_ptr<Elector>> generateTestElectors(int numElectors,
                                                    std::map<WorkerType, double> distribution,
                                                    std::map<WorkerType, double> utilityDistribution);
};
