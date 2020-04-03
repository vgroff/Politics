#include "../../include/politics/PoliticalCompassPointGenerator.hpp"


PoliticalCompassDistribution::PoliticalCompassDistribution(std::vector<Gaussian> gaussians_,
                                                           std::vector<double> baseDist_,
                                                           std::map<PoliticalAxis, std::vector<std::pair<std::vector<double>, double>>> correlations_)
    : gaussians(gaussians_) {
    sumsToOne(baseDist_);
    baseDist = baseDist_;
    for (const auto& correlationPair : correlations_) {
        for (size_t i = 0; i < correlationPair.second.size(); i++) {
            if (sumsToOne(correlationPair.second[i].first) == false) {
                throw std::invalid_argument("correlations do not sum to 1");
            }
        }
    }
    correlations = correlations_;
}

std::pair<size_t, double> PoliticalCompassDistribution::sample(const std::map<PoliticalAxis, size_t>& otherResults) {
    std::vector<std::pair<std::vector<double>, double>> probabilityDists = { {baseDist, 1.0} };
    for (const auto& resultPair : otherResults) {
        PoliticalAxis axis = resultPair.first;
        size_t result = resultPair.second;

        auto dist = correlations.find(axis);
        if (dist != correlations.end()) {
            probabilityDists.push_back(correlations[axis][result]);
        };
    }
    normalise(probabilityDists);
    std::vector<double> dist = coinFlip(probabilityDists);
    size_t gaussianIndex = coinFlip(dist);
    double value = gaussians[gaussianIndex].sample();
    if (value > 1) {
        value = 1;
    } else if (value < -1) {
        value = -1;
    }
    return {gaussianIndex, value};
}

std::vector<Gaussian> PoliticalCompassDistribution::generateGaussians(double mean, double stdDevs, double polarisation) {
    std::vector<Gaussian> result = {
        Gaussian(mean - polarisation, stdDevs),
        Gaussian(mean, stdDevs),
        Gaussian(mean + polarisation, stdDevs)
    };
    return result;
}


PoliticalCompassPointGenerator::PoliticalCompassPointGenerator(std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists) 
{
    if (politicalCompassDists.size() != politicalAxis_size) {
        throw std::invalid_argument("Not enough political compass distributions");
    }
    this->politicalCompassDists = politicalCompassDists;
};

PoliticalCompassPoint PoliticalCompassPointGenerator::sample() {
    std::map<PoliticalAxis, double> ideology = {};
    std::map<PoliticalAxis, size_t> previousResults = {};
    for (auto& axisPair : politicalCompassDists) {
        PoliticalAxis axis = axisPair.first;
        std::pair<size_t, double> result = axisPair.second.sample(previousResults);
        ideology[axis] = result.second;
        previousResults[axis] = result.first;
    }
    PoliticalCompassPoint p(ideology);
    return p;
}

PoliticalCompassPointGenerator PoliticalCompassPointGenerator::testSetup() {
    std::vector<double> evenArray = {1.0/3, 1.0/3, 1.0/3};
    std::vector<double> normalArray = {0.38, 0.24, 0.38};
    std::vector<double> skewProgArray = {0.63, 0.25, 0.12};
    std::vector<double> skewReactionaryArray = {0.12, 0.25, 0.63};
    std::vector<double> skewAwayFromCentre = {0.4, 0.2, 0.4};
    auto capitalistGaussian = PoliticalCompassDistribution::generateGaussians(0.1, 0.2, 0.4);
    auto conservativeGaussian = PoliticalCompassDistribution::generateGaussians(0.1, 0.3, 0.3);
    auto nationalistGaussian = PoliticalCompassDistribution::generateGaussians(0.05, 0.3, 0.35);
    std::map<PoliticalAxis, PoliticalCompassDistribution> politicalCompassDists = {
        {Conservative, PoliticalCompassDistribution(conservativeGaussian, normalArray, {})},
        {Capitalist, PoliticalCompassDistribution(capitalistGaussian, normalArray, {{Conservative, {{skewProgArray, 3}, {evenArray, 0}, {skewReactionaryArray, 3}}}})},
        {Nationalist, PoliticalCompassDistribution(nationalistGaussian, normalArray, {{Conservative, {{skewProgArray, 3}, {normalArray, 0}, {skewReactionaryArray, 3}}},
                                                                                      {Capitalist, {{skewProgArray, 2}, {skewProgArray, 1}, {evenArray, 1}}}})},
    };
    PoliticalCompassPointGenerator p(politicalCompassDists);
    return p;
}