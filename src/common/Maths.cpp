#include<stdexcept>
#include<random>
#include<chrono>
#include "../../include/common/Maths.hpp"

double weightedMovingAvg(double memoryWeight, double oldAverage, double newValue) {
    if (memoryWeight > 1 || memoryWeight < 0) {
        throw std::invalid_argument("Memory weight is an invalid value");
    }
    return memoryWeight*oldAverage + (1 - memoryWeight)*newValue;
}

double random0to1() {
    return (double) std::rand() / RAND_MAX;
}

bool coinFlip(double prob) {
    if (prob > random0to1()) {
        return true;
    }
    return false;
}

size_t coinFlip(const std::vector<double> probabilityDist) {
    double rand = random0to1() + 0.00001;
    double val = 0;
    for (size_t i = 0; i < probabilityDist.size(); i++) {
        val += probabilityDist[i];
        if (val >= rand) {
            return i;
        }
    }
    return probabilityDist.size();
}

bool sumsToOne(const std::vector<double>& dist) {
    double sumDist = 0;
    for (const auto distVal : dist) {
        sumDist += distVal;
    }
    if (std::abs(sumDist - 1) < 0.00001) {
        return true;
    }
    return false;    
}

Gaussian::Gaussian(double mean, double stdDev) {
    setMean(mean);
    setStdDev(stdDev);
    generator = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
}

double Gaussian::sample() {
    return dist(generator);
}

void Gaussian::setMean(double mean) {
    this->mean = mean;
    dist = std::normal_distribution<double>(mean, stdDev);
}

void Gaussian::setStdDev(double stdDev) {
    this->stdDev = stdDev;
    dist = std::normal_distribution<double>(mean, stdDev);
}