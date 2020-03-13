#include "../../include/politics/PoliticalCompassGenerator.hpp"


PoliticalCompassDistribution::PoliticalCompassDistribution(std::array<Gaussian, 3> gaussians_,
                                                           std::array<double, numGaussians> baseDist_,
                                                           std::map<PoliticalAxis, std::array<std::pair<std::array<double, numGaussians>, double>, numGaussians>> correlations_)
    : gaussians(gaussians_) {
    sumsToOne(baseDist_);
    baseDist = baseDist_;
    for (const auto correlationPair : correlations_) {
        for (size_t i = 0; i < numGaussians; i++) {
            if (sumsToOne(correlationPair.second[i].first) == false) {
                throw std::invalid_argument("correlations do not sum to 1");
            }
        }
    }
    correlations = correlations_;
}

double PoliticalCompassDistribution::sample(std::map<PoliticalAxis, size_t> otherResults) {
    std::vector<std::pair<std::array<double, numGaussians>, double>> probabilityDists = { {baseDist, 1.0} };
    for (const auto resultPair : otherResults) {
        PoliticalAxis axis = resultPair.first;
        size_t result = resultPair.second;
        std::pair<std::array<double, numGaussians>, double> dist = correlations[axis][result];
        probabilityDists.push_back(dist);
    }
    normalise(probabilityDists);
    std::array<double, numGaussians> dist = coinFlip(probabilityDists);
    size_t gaussianIndex = coinFlip(dist);
    double value = gaussians[gaussianIndex].sample();
    if (value > 1) {
        value = 1;
    } else if (value < -1) {
        value = -1;
    }
    return value;
}

std::array<Gaussian, 3> PoliticalCompassDistribution::generateGaussians(double mean, double stdDevs, double polarisation) {
    std::array<Gaussian, 3> result = {
        Gaussian(mean - polarisation, stdDevs),
        Gaussian(mean, stdDevs),
        Gaussian(mean+polarisation, stdDevs)
    };
    return result;
}


PoliticalCompassPointGenerator::PoliticalCompassPointGenerator(std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists) 
{
    if (politicalCompassDist.size() != politicalAxis_size) {
        throw std::invalid_argument("Not enough political compass distributions");
    }
    this->politicalCompassDist = politicalCompassDist;
};

PoliticalCompassPoint PoliticalCompassPointGenerator::sample() {
    std::maps<PoliticalAxis, double> ideology = {};
    for (const auto axisPair : politicalCompassDist) {
        double result = axisPair.second.sample();
        ideology[axisPair.first] = result;
    }
    PoliticalCompassPoint p(ideology);
    return p;
}

PoliticalCompassPointGenerator PoliticalCompassPointGenerator::testSetupGenerator() {
    std::array<Gaussian, 3> evenArray = {1.0/3, 1.0/3, 1.0/3};
    std::array<Gaussian, 3> normalArray = {0.4, 0.2, 0.4};
    std::array<Gaussian, 3> skewProgArray = {0.6, 0.25, 0.15};
    std::array<Gaussian, 3> skewReactionaryArray = {0.15, 0.25, 0.6};
    std::array<Gaussian, 3> skewAwayFromCentre = {0.4, 0.2, 0.4};
    auto capitalistGaussian = PoliticalCompassDistribution(0.1, 0.25, 0.35);
    auto conservativeGaussian = PoliticalCompassDistribution(0.1, 0.25, 0.4);
    auto nationalistGaussian = PoliticalCompassDistribution(0.05, 0.35, 0.45);
    std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists = {
        {Capitalist, PoliticalCompassDistribution(capitalistGaussian, evenArray, {})},
        {Conservative, PoliticalCompassDistribution(conservativeGaussian, evenArray, {{Capitalist, {{skewProgArray, 2}, {evenArray, 0}, {skewReactionaryArray, 2}})}}},
        {Nationalist, PoliticalCompassDistribution(nationalistGaussian, normalArray, {{Capitalist, {{skewProgArray, 2}, {skewProgArray, 1}, {skewReactionaryArray, 0}})
                                                                                      {Conservative, {{skewProgArray, 3}, {normalArray, 0}, {skewReactionaryArray, 3}})}}},
    }
    PoliticalCompassPointGenerator(politicalCompassDists);
}