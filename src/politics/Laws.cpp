#include "../../include/politics/Laws.hpp"

BudgetLaws::BudgetLaws(std::map<Commodity, std::map<CommodityLevel, double>> consumptionTax_,
                       std::map<double, double> incomeTaxBrackets_,
                       double dividendTax,
                       double capitalGainsTax) 
    : capitalGainsTax(capitalGainsTax),
    dividendTax(dividendTax) {
    setNewConsumptionTax(consumptionTax_);
    setNewIncomeTaxBrackets(incomeTaxBrackets_);
}

void BudgetLaws::setNewIncomeTaxBrackets(std::map<double, double> newIncomeTax) {
    for (const auto& [taxLevel, taxRate] : newIncomeTax) {
        if (taxLevel < 0) {
            throw std::invalid_argument("Tax levels must be positive");
        }
        if (taxRate < 0 || taxRate > 1) {
            throw std::invalid_argument("Tax rate must be between 0 and 1");
        }
    }
    incomeTaxBrackets = newIncomeTax;
}

void BudgetLaws::setNewConsumptionTax(std::map<Commodity, std::map<CommodityLevel, double>> newConsumptionTax) {
    if (newConsumptionTax.size() != NUM_COMMODITIES) {
        throw std::invalid_argument("Consumption tax does not have the correct number of commodities");
    }
    for (const auto& [commodity, commodityLevelTax] : newConsumptionTax) {
        if (commodityLevelTax.size() != NUM_COMMODITY_LEVELS) {
            throw std::invalid_argument("Consumption tax does not have the correct number of commodity levels");
        }
        for (const auto& [commodityLevel, commodityTax] : commodityLevelTax)
            if (commodityTax < 0) {
                throw std::invalid_argument("Consumption tax must be positive (less than 1 if a subsidy)");
            }
        }
    }
    consumptionTax = newConsumptionTax;
}

std::map<Commodity, std::map<CommodityLevel, double>> BudgetLaws::getNewPrices(std::map<Commodity, std::map<CommodityLevel, double>> oldPrices) {
    std::map<Commodity, std::map<CommodityLevel, double>> newPrices = {};
    for (auto& [commodity, commodityPrices] : oldPrices) {
        newPrices[commodity] = {}; 
        for (auto& [commodityLevel, price] : commodityPrices) {
            newPrices[commodity][commodityLevel] = price * consumptionTax[commodity][commodityLevel]; 
        }
    }
    return newPrices;
}

double BudgetLaws::getIncomeTax(double income) {
    double tax = 0;
    auto it = incomeTaxBrackets.begin()
    for (const auto& [taxLevel, taxRate] : incomeTaxBrackets) {
        it++;
        double nextTaxLevel = it->first;
        if (nextTaxLevel < income) {
            tax += (nextTaxLevel - taxLevel) * taxRate
        } else {
            tax += (income - taxLevel) * taxRate
            break;
        }
    }
    return tax;
}

double BugetLaws::getWelfare() {
    return welfareAmount;
}

double BugetLaws::getDividendTax(double dividend) {
    return dividendTax * dividend;
}

double BugetLaws::getCapitalGainsTax(double capitalGains) {
    return capitalGainsTax * capitalGains;
}