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
    double getNumWorkers();
    void setNumWorkers(double nWorkers);
    double getProductionCapacity();
    double getNumJobs();
    std::map<WorkerType, double> getWorkerDistribution();
    std::map<WorkerType, double> getWages();

    static Industry testSetup();
};