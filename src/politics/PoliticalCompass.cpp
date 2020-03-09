#include "../../include/politics/PoliticalCompass.hpp"
#include<cmath>

PoliticalCompassPoint::PoliticalCompassPoint(std::map<PoliticalAxis, double> point,
                                             std::map<PoliticalAxis, double> strengths) {
    if (point.size() == politicalAxis_size && strengths.size() == politicalAxis_size) {
        point = point;
        strengths = strengths
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
        sum += absDiff * strengths.at(pointPair.first);
        sumStrengths += strengths.at(pointPair.first);
    }
    return sum/sumStrengths;
}
