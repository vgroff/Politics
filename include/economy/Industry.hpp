#pragma once

#include <map>
#include "../electors/Elector.hpp"

class Industry {
private:
    double productivity;
    double productionCapacity;
    double numWorkers;
    double wagePerWorker;
    double currentTechnology;
    const double productivityToConstantCapital = 1/6.0;
    std::map<WorkerType, double> workerDistribution = {};
    std::map<WorkerType, double> payDistribution = {};

public:
    Industry(double productivity,
            double productionCapacity,
            double wagePerWorker,
            std::map<WorkerType, double> workerDistribution,
            std::map<WorkerType, double> payDistribution);

    void setNewWorkerPayDists(std::map<WorkerType, double> workerDistribution,
                              std::map<WorkerType, double> payDistribution);
    double getProduction();
    double getTotalWages();
    double getProfit();
    double getNumWorkers();
    double getWagePerWorker();
    double getProductionCapacity();
    double getProductivityToConstantCapital();
    double getNumJobs();
    double getProductivity();
    double getConstantCapital();
    void makeProductivityInvestement(double investement);
    void makeProductionCapacityInvestement(double investement);
    Industry theoreticalProductivityInvestement(double investement);
    Industry theoreticalProductionCapacityInvestement(double investement);
    double getTheoreticalConstantCapital(double theoreticalProductivity);
    std::map<WorkerType, double> getWorkerDistribution();
    std::map<WorkerType, double> getWages();

    void setNumWorkers(double nWorkers);
    void setCurrentTechnology(double currentTech);

    static double getProduction(double productivity, double numWorkers);
    static double getTotalWages(double wagePerWorker, double numWorkers);
    static double getConstantCapital(double productivity, double productivityToConstantCapital, double numWorkers);
    static double getProfit(double production, double wages, double constantCapital);

    static Industry testSetup();
};