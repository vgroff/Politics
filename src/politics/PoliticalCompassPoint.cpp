#include<cmath>
#include<sstream>
#include<iostream>
#include<iomanip>
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

std::string politicalAxisToStringShort(PoliticalAxis politicalAxis){
    if (politicalAxis == Capitalist) {
        return "Cap";
    } else if (politicalAxis == Conservative) {
        return "Con";
    } else if (politicalAxis == Nationalist) {
        return "Nat";
    }
    return "Unknown Political Axis";
}

PoliticalCompassPoint::PoliticalCompassPoint(std::map<PoliticalAxis, double> point_) {
    if (point_.size() == politicalAxis_size) {
        point = point_;
    } else {
        throw std::invalid_argument("Point and strengths map are not the correct size");
    }
}

double PoliticalCompassPoint::getValue(PoliticalAxis axis) {
    return point.at(axis);
}

double PoliticalCompassPoint::getDistanceTo(PoliticalCompassPoint point2, std::map<PoliticalAxis, double> strengths) {
    double sum = 0;
    double sumStrengths = 0;
    for (const auto& pointPair : point) {
        PoliticalAxis axis = pointPair.first;
        double value = pointPair.second;
        double absDiff = std::abs(value - point2.getValue(axis));
        double strength = strengths.at(pointPair.first);
        sum += std::pow((absDiff/2), 1.3) * strength; // divide by 2 to force it between 0 and 1 before exponentiating
        sumStrengths += strength;
    }
    return sum/sumStrengths;
}

std::string PoliticalCompassPoint::toString() {
    std::ostringstream strStream;
    strStream << std::setprecision(2);
    for (const auto& pointPair : point) {
        strStream << politicalAxisToStringShort(pointPair.first) << ": " << pointPair.second;
        if (static_cast<int>(pointPair.first) != politicalAxis_size - 1) {
            strStream << ", ";
        }
    }
    return strStream.str();
}

std::map<PoliticalAxis, double> PoliticalCompassPoint::getDefaultStrengths() {
    return {
        {Conservative, 0.7},
        {Capitalist, 1.2},
        {Nationalist, 1.1}
    };
}