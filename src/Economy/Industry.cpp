#include "../../include/Economy/Industry.hpp"
#include <cmath>
#include <stdexcept>

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
    totalWages(totalWages),
    constantCapitalCost(constantCapitalCost)
    {
    setNewWorkerPayDists(workerDistribution, payDistribution);
}
#include<iostream>
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
        std::cout << paySum << ", "  << workerSum << ", " << std::abs(paySum - 1.0) << std::endl;
        if (std::abs(workerSum - 1.0) > 0.0001 || std::abs(paySum - 1.0) > 0.0001) {
            throw std::invalid_argument("ratios don't sum to 1");
        }
    }

    this->payDistribution = payDistribution;
    this->workerDistribution = workerDistribution;
}