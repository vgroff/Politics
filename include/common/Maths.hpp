#pragma once
#include<cmath>
#include<map>
#include<set>
#include<vector>
#include<iostream>
#include<array>
#include<random>

double weightedMovingAvg(double memoryWeight, double oldAverage, double newValue);
bool coinFlip(double prob);
double random0to1();

class Gaussian {
private:
    double mean;
    double stdDev;
    std::default_random_engine generator;
    std::normal_distribution<double> dist;
public:
    Gaussian(double mean, double stdDev);
    void setMean(double stdDev);
    void setStdDev(double stdDev);
    double getMean();
    double getStdDev();
    double sample();
};

template<typename A>
void throwIfInconsistent(const std::map<A, double>& dist1,
                        const std::map<A, double>& dist2,
                        bool sumsToOne1,
                        bool sumsToOne2) {
    if (dist1.size() != dist2.size()) {
        throw std::invalid_argument("Pay and worker distribution maps are different sizes");
    }
    std::set<A> set;
    double sum1 = 0;
    double sum2 = 0;
    for (const auto& distPair : dist1) {
        if (set.find(distPair.first) == set.end()) {
            set.insert(distPair.first);
        } else {
            throw std::invalid_argument("A worker type appears twice");
        } 
        sum1 += distPair.second;
        if (dist2.find(distPair.first) != dist2.end()) {
            sum2 += dist2.at(distPair.first);
        } else {
            throw std::invalid_argument("Mismatch worker distribution maps");
        }
    }
    if ((std::abs(sum1 - 1.0) > 0.0001 && sumsToOne1) || (std::abs(sum2 - 1.0) > 0.0001 && sumsToOne2)) {
        throw std::invalid_argument("Ratios don't sum to 1");
    }
}

bool sumsToOne(const std::vector<double>& dist);

template<typename A>
bool sumsToOne(std::vector<std::pair<A, double>> dist) {
    double sumDist = 0;
    for (const auto distPair : dist) {
        sumDist += distPair.second;
    }
    if (std::abs(sumDist - 1) < 0.00001) {
        return true;
    }
    return false;
}

template<typename A>
bool sumsToOne(std::map<A, double> dist) {
    double sumDist = 0;
    for (const auto distPair : dist) {
        sumDist += distPair.second;
    }
    if (std::abs(sumDist - 1) < 0.00001) {
        return true;
    }
    return false;
}

template<typename A, size_t SIZE>
bool sumsToOne(std::array<A, SIZE> dist) {
    double sumDist = 0;
    for (size_t i = 0; i < SIZE; i++) {
        sumDist += dist[i];
    }
    if (std::abs(sumDist - 1) < 0.00001) {
        return true;
    }
    return false;
}


template<typename A>
void normalise(std::vector<std::pair<A, double>>& dist) {
    double sumDist = 0;
    for (size_t i = 0; i < dist.size(); i++) {
        sumDist += dist[i].second;
    }
    for (size_t i = 0; i < dist.size(); i++) {
        dist[i].second = dist[i].second / sumDist;
    }
}


template<typename A>
A coinFlip(const std::map<A, double>& probabilityDist) {
    double rand = random0to1() + 0.00001;
    double val = 0;
    A currentVal;
    for (const auto& [value, probability] : probabilityDist) {
        currentVal = value;
        val += probability;
        if (val >= rand) {
            return value;
        }
    }
    return currentVal;
}

size_t coinFlip(const std::vector<double> probabilityDist);

template<typename A>
A coinFlip(const std::vector<std::pair<A, double>>& probabilityDist) {
    double rand = random0to1()+0.00001;
    double val = 0;
    for (const auto& [value, probability] : probabilityDist) {
        val += probability;
        if (val >= rand) {
            return value;
        }
    }
    return probabilityDist[probabilityDist.size() - 1].first;
}

