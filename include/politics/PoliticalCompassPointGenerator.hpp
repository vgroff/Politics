#pragma once
#include "PoliticalCompassPoint.hpp"
#include "../common/Maths.hpp"
#include<array>

class PoliticalCompassDistribution {
private:
    static const size_t numGaussians = 3;
    std::array<Gaussian, numGaussians> gaussians;
    std::array<double, numGaussians> baseDist;
    std::map<PoliticalAxis, std::array<std::pair<std::array<double, numGaussians>, double>, numGaussians>> correlations;
public:
    PoliticalCompassDistribution(std::array<Gaussian, numGaussians> gaussians,
                                 std::array<double, numGaussians> baseDist,
                                 std::map<PoliticalAxis, std::array<std::pair<std::array<double, numGaussians>, double>, numGaussians>> correlations);
    double sample(std::map<PoliticalAxis, size_t> otherResults);

    static std::array<Gaussian, 3> generateGaussians(double mean, 
                                                    double stdDevs, 
                                                    double polarisation);
};

class PoliticalCompassPointGenerator {
private:
    std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists;
public:
    PoliticalCompassPointGenerator(std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists);
    PoliticalCompassPoint sample();

    
    static PoliticalCompassPointGenerator testSetupGenerator();
};

