#include "../../include/economy/Industry.hpp"
#include "../../include/common/Maths.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

Industry::Industry(double productivity,
                   double productionCapacity,
                   double wagePerWorker,
                   std::map<WorkerType, double> workerDistribution,
                   std::map<WorkerType, double> payDistribution)
    : productivity(productivity),
    productionCapacity(productionCapacity),
    wagePerWorker(wagePerWorker)
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

void Industry::printStatus() {
    std::cout << "Industry Report: " << std::endl;
    std::cout << "Productivity: " << getProductivity()  << std::endl;
    std::cout << "Production Capacity: " << getProductionCapacity() << " (Workers: " << getNumWorkers() << ")" << std::endl;
}

void Industry::setNumWorkers(double nWorkers) {
    if (nWorkers > getNumJobs()) {
        throw(std::invalid_argument("Industry can not support this many workers"));
    }
    numWorkers = nWorkers;
}

double Industry::getNumWorkers() {
    return numWorkers;
}

double Industry::getProduction() {
    return Industry::getProduction(productivity, numWorkers);
}

double Industry::getTotalWages() {
    return Industry::getTotalWages(wagePerWorker, numWorkers);
}

double Industry::getProductionCapacity() {
    return productionCapacity;
}

double Industry::getProductivity() {
    return productivity;
}

double Industry::getNumJobs() {
    return productionCapacity;
}

double Industry::getWagePerWorker() {
    return wagePerWorker;
}

double Industry::getProductivityToConstantCapital() {
    return productivityToConstantCapital;
}

double Industry::getConstantCapital() {
    return productivity*numWorkers*productivityToConstantCapital;
}

void Industry::setCurrentTechnology(double newTech) {
    currentTechnology = newTech;
} 

Industry Industry::theoreticalProductivityInvestement(double investement) {
    Industry i(*this);
    i.makeProductivityInvestement(investement);
    return i;
}

void Industry::makeProductivityInvestement(double investement) {
    double diff = (currentTechnology - productivity);
    double fracDiff = diff / currentTechnology;
    double adjInvestement = (1 - 0.35*fracDiff)*investement/productionCapacity; // Punishment for pushing the bounds of technology
    double sigmoid = 1/(1 + std::exp(-adjInvestement*MONTHS_PER_TURN/(currentTechnology)));
    double additionalProductivity = diff * (2*sigmoid - 1); 
    double newProductivity = productivity + additionalProductivity;
    productivity = newProductivity;
}

Industry Industry::theoreticalProductionCapacityInvestement(double investement) {
    Industry i(*this);
    i.makeProductionCapacityInvestement(investement);
    return i;
}

void Industry::makeProductionCapacityInvestement(double investement) {
    productionCapacity += investement*investementToProductionCapacityRatio;
}

double Industry::getTheoreticalConstantCapital(double theoreticalProductivity) {
    return theoreticalProductivity*productivityToConstantCapital*numWorkers;
}

std::map<WorkerType, double> Industry::getWorkerDistribution() {
    return workerDistribution;
}

double Industry::getProfit() {
    return Industry::getProfit(getProduction(), getTotalWages(), getConstantCapital());
}

double Industry::getProduction(double productivity, double numWorkers) {
    return productivity*numWorkers;
}

double Industry::getTotalWages(double wagePerWorker, double numWorkers) {
    return wagePerWorker*numWorkers;
}

double Industry::getConstantCapital(double productivity, double productivityToConstantCapital, double numWorkers) {
    return productivity*productivityToConstantCapital*numWorkers;
}

double Industry::getProfit(double production, double totalWages, double constantCapital) {
    return production - (totalWages + constantCapital);
}

Industry Industry::testSetup() {
    std::map<WorkerType, double> workerDistribution = { {HighSkilled, 0.2}, {Skilled, 0.4}, {Unskilled, 0.4} };
    std::map<WorkerType, double> payDistribution = { {HighSkilled, 5.0/8}, {Skilled, 2.0/8}, {Unskilled, 1.0/8} };
    Industry i(1, 37, 0.65, workerDistribution, payDistribution);
    i.setCurrentTechnology(1.05);
    return i;
}
