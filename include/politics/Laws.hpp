#pragma once
#include "../electors/Electors.hpp"
#include "../electors/Utility.hpp"
#include "./PoliticalCompassPoint.hpp"
#include<vector>
#include<map>

struct BudgetLaws {
private:
    // Subsidies in here too
    std::map<Commodity, std::map<CommodityLevel, double>> consumptionTax;
    std::map<double, double> incomeTaxBrackets;
    double dividendTax;
    double capitalGainsTax;

    double welfareAmount;

    double getRadicalness(const& BudgetLaws);
    double getCapitalismness(const std::map<WorkerType, double>& wages);
public:
    BudgetLaws(std::map<Commodity, std::map<CommodityLevel, double>>  consumptionTax,
               std::map<double, double> incomeTaxBrackets,
               double dividendTax,
               double capitalGainsTax);

    void setNewConsumptionTax(std::map<Commodity, std::map<CommodityLevel, double>> newConsumptionTax);
    void setNewIncomeTaxBrackets(std::map<double, double> newIncomeTax);

    double getIncomeTax(double income);
    double getDividendTax(double dividend);
    double getCapitalGainsTax(double capitalGains);
    double getWelfare();
    std::map<Commodity, std::map<CommodityLevel, double>> getNewPrices(std::map<Commodity, std::map<CommodityLevel, double>> oldPrices);

    PoliticalPointCompass getIdeology(const& BudgetLaws, const std::map<WorkerType, double>& wages);
};

struct LabourLaws {
    double strength;
};

struct CriminalLaws {
    bool legalDrugs;
};