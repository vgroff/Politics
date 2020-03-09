#pragma once
#include<map>

enum PoliticalAxis {
    Capitalist = 0, // Socialist
    Nationalist, // Internationalist
    Conservative, // Radical
    Moralist, // Liberal
    Engaged, // Disinterested 
    Loyalist // Non-loyalist (loyalty to party)
};
const int politicalAxis_size = Loyalist + 1;

class PoliticalCompassPoint {
private:
    std::map<PoliticalAxis, double> point;
    std::map<PoliticalAxis, double> strengths;
public:
    PoliticalCompassPoint(std::map<PoliticalAxis, double> point,
                          std::map<PoliticalAxis, double> strengths);
    double getValue(PoliticalAxis axis);
    double getDistanceTo(PoliticalCompassPoint point2);
};
