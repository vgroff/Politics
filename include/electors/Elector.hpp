#pragma once

#include<string>
#include"../common/Date.hpp"

enum WorkerType {
    Unemployed = 0,
    Unskilled,
    Skilled,
    HighSkilled,
    Capitalist // Forgive me
};

enum WorkerEducation {
    School = 0,
    HighSchool,
    University
};
const WorkerEducation WORST_EDUCATION = School;
const WorkerEducation BEST_EDUCATION = University;

std::string workerTypeToString(WorkerType workerType);
std::string workerEducationToString(WorkerEducation workerEducation);

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
    WorkerType workerType = Unemployed;
    WorkerEducation workerEducation;
public:
    double setUtility(double newUtility);
    double getCurrentUtility();
    double getShortTermUtility();
    double getLongTermUtility();
    WorkerType getWorkerType();
    WorkerEducation getWorkerEducation();
    void setWorkerType(WorkerType workerType);
    bool canWorkJob(WorkerType jobType);
    Elector(WorkerEducation workerEducation, double longTermUtility);

    static bool canWorkJob(WorkerEducation workerEducation, WorkerType workerType);

    static std::vector<Elector> generateTestElectors(int numElectors,
                                                    std::map<WorkerEducation, double> distribution,
                                                    std::map<WorkerEducation, double> utilityDistribution);
};