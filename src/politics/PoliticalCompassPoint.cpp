#include<cmath>
#include "../../include/politics/PoliticalCompass.hpp"

PoliticalCompassPoint::PoliticalCompassPoint(std::map<PoliticalAxis, double> point) {
    if (point.size() == politicalAxis_size) {
        point = point;
    } else {
        throw std::invalid_argument("Point and strengths map are not the correct size");
    }
}

double PoliticalCompassPoint::getValue(PoliticalAxis axis) {
    return point.at(axis);
}

double PoliticalCompassPoint::getDistanceTo(PoliticalCompassPoint point2) {
    double sum = 0;
    double sumStrengths = 0;
    for (const auto pointPair : point) {
        PoliticalAxis axis = pointPair.first;
        double value = pointPair.second;
        double absDiff = std::abs(value - point2.getValue(axis));
        double strength = 1 + absDiff/1.25
        sum += absDiff * strength;
        sumStrengths += strength;
    }
    return sum/sumStrengths;
}
