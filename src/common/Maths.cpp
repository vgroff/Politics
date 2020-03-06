#include<stdexcept>
#include "../../include/common/Maths.hpp"

double weightedMovingAvg(double memoryWeight, double oldAverage, double newValue) {
    if (memoryWeight > 1 || memoryWeight < 0) {
        throw std::invalid_argument("Memory weight is an invalid value");
    }
    return memoryWeight*oldAverage + (1 - memoryWeight)*newValue;
}
