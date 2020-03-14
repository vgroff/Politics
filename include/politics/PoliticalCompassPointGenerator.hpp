#pragma once
#include "PoliticalCompassPoint.hpp"
#include "../common/Maths.hpp"
#include<array>

class PoliticalCompassDistribution {
private:
    std::vector<Gaussian> gaussians;
    std::vector<double> baseDist;
    std::map<PoliticalAxis, std::vector<std::pair<std::vector<double>, double>>> correlations;
public:
    PoliticalCompassDistribution(std::vector<Gaussian> gaussians,
                                 std::vector<double> baseDist,
                                 std::map<PoliticalAxis, std::vector<std::pair<std::vector<double>, double>>> correlations);
    std::pair<size_t, double> sample(const std::map<PoliticalAxis, size_t>& otherResults);

    static std::vector<Gaussian> generateGaussians(double mean, 
                                                    double stdDevs, 
                                                    double polarisation);
};

class PoliticalCompassPointGenerator {
private:
    std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists;
public:
    PoliticalCompassPointGenerator(std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists);
    PoliticalCompassPoint sample();

    
    static PoliticalCompassPointGenerator testSetup();
};

