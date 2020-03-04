#include "../include/Nation.hpp"
#include<iostream>

Nation::Nation(std::string name, double population, Industry privateIndustry)
    : name(name), population(population), privateIndustry(privateIndustry) {    

}

void Nation::runIndustryTurn() {
    double privateProfit = privateIndustry.getProfit();
    std::cout << "Profit is: " << privateProfit << std::endl;
}

Nation Nation::testSetupSingleNation() {
    Nation nation("United Kingdom",
                  60,
                  Industry::testSetup());
    return nation;
}
