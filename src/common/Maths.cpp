#include<stdexcept>
#include<random>
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