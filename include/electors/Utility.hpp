#pragma once
#include<map>

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

std::map<Commodity, std::map<CommodityLevel, double>> flatConsumptionTax(double rate);

double getUtility(double income);
double inverseUtility(double income);