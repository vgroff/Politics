#include "../../include/Economy/Industry.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

Industry::Industry(double productivity,
                   double productionCapacity,
                   double numWorkers,
                   double totalWages,
                   double constantCapitalCost,
                   std::map<WorkerType, double> workerDistribution,
                   std::map<WorkerType, double> payDistribution)
    : productivity(productivity),
    productionCapacity(productionCapacity),
    numWorkers(numWorkers),
    wagePerWorker(wagePerWorker),
    constantCapitalCost(constantCapitalCost)
    {
    setNewWorkerPayDists(workerDistribution, payDistribution);
}

void Industry::setNewWorkerPayDists(std::map<WorkerType, double> workerDistribution,
                                    std::map<WorkerType, double> payDistribution) {
    if (payDistribution.size() != workerDistribution.size()) {
        throw std::invalid_argument("Pay and worker distribution maps are different sizes");
    }
    if (workerDistribution.size() > 0) {
        double paySum = 0;
        double workerSum = 0;
        for (const auto& workerDistPair : workerDistribution) {
            workerSum += workerDistPair.second;
            if (payDistribution.find(workerDistPair.first) != payDistribution.end()) {
                paySum += payDistribution.at(workerDistPair.first);
            } else {
                throw std::invalid_argument("Mismatch between pay and worker distribution maps");
            }
        }

        if (std::abs(workerSum - 1.0) > 0.0001 || std::abs(paySum - 1.0) > 0.0001) {
            throw std::invalid_argument("ratios don't sum to 1");
        }
    }

    this->payDistribution = payDistribution;
    this->workerDistribution = workerDistribution;
}

double Industry::getProduction() {
    return productivity*numWorkers;
}

double Industry::getTotalWages() {
    return wagePerWorker*numWorkers;
}

double Industry::getProfit() {
    double production = getProduction();
    double wages = getTotalWages();
    std::cout << "Production i
    return production - (wages + constantCapitalCost);
}

Industry Industry::testSetup() {
    std::map<WorkerType, double> workerDistribution = { {HighSkilled, 0.2}, {Skilled, 0.4}, {Unskilled, 0.4} };
    std::map<WorkerType, double> payDistribution = { {HighSkilled, 4.0/7}, {Skilled, 2.0/7}, {Unskilled, 1.0/7} };
    Industry i(1, 39, 39, 0.65, 5, workerDistribution, payDistribution);
    return i;
}
