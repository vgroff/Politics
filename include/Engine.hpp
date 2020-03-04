#pragma once

#include "Common/Date.hpp"
#include "Nation.hpp"

class Engine {
private:
    date currentDate;
    Nation playerNation = Nation::testSetupSingleNation();
public:
    Engine(date startingDate);
    void playTurn();

    static Engine testSetup();
};