#pragma once

#include <map>
#include "../electors/Elector.hpp"

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
    std::map<WorkerType, double> getWages();

    static Industry testSetup();
};