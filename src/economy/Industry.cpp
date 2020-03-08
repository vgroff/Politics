#include "../../include/economy/Industry.hpp"
#include "../../include/common/Maths.hpp"
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

    if (workerDistribution.size() > 0) {
        throwIfInconsistent(workerDistribution, payDistribution, true, true);
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

void Industry::setNumWorkers(double numWorkers) {
    if (numWorkers > getNumJobs()) {
        throw(std::invalid_argument("Industry can not support this many workers"));
    }
    numWorkers = numWorkers;
}

double Industry::getNumWorkers() {
    return numWorkers;
}

double Industry::getProduction() {
    return productivity*numWorkers;
}

double Industry::getTotalWages() {
    return wagePerWorker*numWorkers;
}

double Industry::getProductionCapacity() {
    return productionCapacity;
}

double Industry::getNumJobs() {
    return productionCapacity;
}

std::map<WorkerType, double> Industry::getWorkerDistribution() {
    return workerDistribution;
}

double Industry::getProfit() {
    double production = getProduction();
    double wages = getTotalWages();
    std::cout << production << ", " << wages << std::endl;
    return production - (wages + constantCapitalCost);
}

Industry Industry::testSetup() {
    std::map<WorkerType, double> workerDistribution = { {HighSkilled, 0.2}, {Skilled, 0.4}, {Unskilled, 0.4} };
    std::map<WorkerType, double> payDistribution = { {HighSkilled, 5.0/8}, {Skilled, 2.0/8}, {Unskilled, 1.0/8} };
    Industry i(1, 50, 50, 0.65, 5, workerDistribution, payDistribution);
    return i;
}
