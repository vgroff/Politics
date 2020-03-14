#pragma once
#include<map>

enum PoliticalAxis {
    Capitalist = 0, // Socialist
    Conservative, // Radical
    Nationalist, // Internationalist
    Authoritarian, // Liberal
    Engaged, // Disinterested 
    Loyalist // Non-loyalist (loyalty to party)
};
const int politicalAxis_size = Nationalist + 1;
std::string politicalAxisToString(PoliticalAxis politicalAxis);

class PoliticalCompassPoint {
private:
    std::map<PoliticalAxis, double> point;
    std::map<PoliticalAxis, double> strengths;
public:
    PoliticalCompassPoint(std::map<PoliticalAxis, double> point, std::map<PoliticalAxis, double> strengths);
    double getValue(PoliticalAxis axis);
    double getDistanceTo(PoliticalCompassPoint point2);
    std::string toString();

    static std::map<PoliticalAxis, double> getDefaultStrengths();
};
