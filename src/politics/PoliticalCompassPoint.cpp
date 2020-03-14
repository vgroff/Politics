#include<cmath>
#include<sstream>
#include<iostream>
#include "../../include/politics/PoliticalCompassPoint.hpp"


std::string politicalAxisToString(PoliticalAxis politicalAxis){
    if (politicalAxis == Capitalist) {
        return "Capitalist";
    } else if (politicalAxis == Conservative) {
        return "Conservative";
    } else if (politicalAxis == Nationalist) {
        return "Nationalist";
    }
    return "Unknown Political Axis";
}

PoliticalCompassPoint::PoliticalCompassPoint(std::map<PoliticalAxis, double> point_,
                                             std::map<PoliticalAxis, double> strengths_) {
    if (point_.size() == politicalAxis_size && strengths_.size() == politicalAxis_size) {
        point = point_;
        strengths = strengths_;
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
        double strength = strengths.at(pointPair.first);
        sum += std::pow((absDiff/2), 1.35) * strength; // divide by 2 to force it between 0 and 1 before exponentiating
        sumStrengths += strength;
    }
    return sum/sumStrengths;
}

std::string PoliticalCompassPoint::toString() {
    std::ostringstream strStream;
    for (const auto pointPair : point) {
        strStream << politicalAxisToString(pointPair.first) << ":" << pointPair.second;
        if (static_cast<int>(pointPair.first) != politicalAxis_size - 1) {
            strStream << ", ";
        }
    }
    return strStream.str();
}

std::map<PoliticalAxis, double> PoliticalCompassPoint::getDefaultStrengths() {
    return {
        {Capitalist, 1.2},
        {Conservative, 0.85},
        {Nationalist, 1.1}
    };
}