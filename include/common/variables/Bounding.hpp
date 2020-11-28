#pragma once
#ifndef BOUNDING
#define BOUNDING

#include<utility>

template <class T>
bool exceedsBounds(T input, std::pair<T, T> bounds);

template <class T>
bool exceedsHigherBound(T input, T higherBound);

template <class T>
bool exceedsLowerBound(T input, T lowerBound);


template<>
inline bool exceedsHigherBound(double input, double higherBound) {
    if (input > higherBound) {
        return true;
    }
    return false;
};

template<>
inline bool exceedsLowerBound(double input, double lowerBound) {
    if (input < lowerBound) {
        return true;
    }
    return false;
};

template<>
inline bool exceedsBounds(double input, std::pair<double, double> bounds) {
    return exceedsHigherBound(input, bounds.first) || exceedsLowerBound(input, bounds.second);
}

#endif
