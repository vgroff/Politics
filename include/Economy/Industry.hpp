#pragma once

#include <map>
#include "../Pop.hpp"

class Industry {
private:
    double productivity;
    double productionCapacity;
    double numWorkers;
    double totalWages;
    double constantCapitalCost;
    std::map<WorkerType, double> workerDistribution;
    std::map<WorkerType, double> payDistribution;

public:
    double getProduction();
    Industry(double productivity,
            double productionCapacity,
            double numWorkers,
            double totalWages,
            double constantCapitalCost,
            std::map<WorkerType, double> workerDistribution,
            std::map<WorkerType, double> payDistribution);
    void setNewWorkerPayDists(std::map<WorkerType, double> workerDistribution,
                               std::map<WorkerType, double> payDistribution);
};