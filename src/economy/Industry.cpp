#include "../../include/economy/Industry.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

Industry::Industry(double productivity,
                   double productionCapacity,
                   double numWorkers,
                   double wagePerWorker,
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

std::map<WorkerType, double> Industry::getWages() {

    double sumPayRatios = 0;
    for (const auto payPair : payDistribution) {
        WorkerType workerType = payPair.first;
        double pay = payPair.second;
        double workers = workerDistribution.at(payPair.first);

        sumPayRatios += pay * workers;
    }

    std::map<WorkerType, double> wages = {};
    for (const auto payPair : payDistribution) {
        WorkerType workerType = payPair.first;
        double pay = payPair.second;
        double workers = workerDistribution.at(payPair.first);

        double payProportionForWorkerType = (pay*workers)/sumPayRatios;
        double numWorkerTypeWorkers = (numWorkers*workers);
        wages[workerType] = getTotalWages() * payProportionForWorkerType / numWorkerTypeWorkers;
    }
    return wages;
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
    std::cout << production << ", " << wages << std::endl;
    return production - (wages + constantCapitalCost);
}

Industry Industry::testSetup() {
    std::map<WorkerType, double> workerDistribution = { {HighSkilled, 0.2}, {Skilled, 0.4}, {Unskilled, 0.4} };
    std::map<WorkerType, double> payDistribution = { {HighSkilled, 4.0/7}, {Skilled, 2.0/7}, {Unskilled, 1.0/7} };
    Industry i(1, 39, 39, 0.65, 5, workerDistribution, payDistribution);
    return i;
}
