#pragma once

#include<string>


enum WorkerType {
    Unskilled = 0,
    Skilled,
    HighSkilled
};

std::string workerTypeToString(WorkerType workerType);


class Elector {
private:
    double currentUtility;
public:
    double setUtility(double income);
    double getUtility();
};