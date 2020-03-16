#pragma once

enum Commodity {
    Food = 0,
    Housing,
    Clothing,
    Healthcare,
};
const int NUM_COMMODITIES = Healthcare + 1;

enum CommodityLevel {
    Basic = 0,
    Quality,
    Luxury,
};
const int NUM_COMMODITY_LEVELS = Luxury + 1;

double getUtility(double income);
double inverseUtility(double income);