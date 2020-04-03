#include "../../include/electors/Utility.hpp"

std::map<Commodity, std::map<CommodityLevel, double>> flatConsumptionTax(double rate) {
    std::map<Commodity, std::map<CommodityLevel, double>> consumptionTaxMap;
    for (size_t i = 0; i < NUM_COMMODITIES; i++) {
        Commodity commodity = Commodity(i);
        consumptionTaxMap[commodity] = {}; 
        for (size_t j = 0; j < NUM_COMMODITY_LEVELS; j++) {
            CommodityLevel commodityLevel = CommodityLevel(j);
            consumptionTaxMap[commodity][commodityLevel] = rate; 
        }
    }
    return consumptionTaxMap;
}

double getUtility(double income) {
    return 1 - 1/(1+(income*2));
}
// y = 1 - 1/(1+2x) => 1/(1-y) = 1+2x => 1/2*(1/(1-y) - 1)
double inverseUtility(double utility) {
    return 0.5*(1.0/(1-utility) - 1);
}