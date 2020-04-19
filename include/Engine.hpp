#pragma once

#include "common/Date.hpp"
#include "Nation.hpp"

class Engine {
private:
    time_point currentDate;
    
public:
    Engine(time_point startingDate);
    void playTurn();
    Nation playerNation = Nation::testSetupSingleNation();

    static Engine testSetup();
};