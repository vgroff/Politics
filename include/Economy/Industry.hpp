#pragma once

#include <map>
#include "../Pop.hpp"

class Industry {
private:
    double productivity;
    double productionCapacity;
    double numWorkers;
    double wagePerWorker;
    double constantCapitalCost;
    std::map<WorkerType, double> workerDistribution = {};
    std::map<WorkerType, double> payDistribution = {};

public:
    Industry(double productivity,
            double productionCapacity,
            double numWorkers,
            double wagePerWorker,
            double constantCapitalCost,
            std::map<WorkerType, double> workerDistribution,
            std::map<WorkerType, double> payDistribution);

    void setNewWorkerPayDists(std::map<WorkerType, double> workerDistribution,
                               std::map<WorkerType, double> payDistribution);
    double getProduction();
    double getTotalWages();
    double getProfit();

    static Industry testSetup();
};