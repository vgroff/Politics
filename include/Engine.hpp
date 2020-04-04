#pragma once

#include "common/Date.hpp"
#include "Nation.hpp"


class Engine {
private:
    time_point currentDate;
    Nation playerNation = Nation::testSetupSingleNation();
public:
    Engine(time_point startingDate);
    void playTurn();
    void setupGUI();

    static Engine testSetup();
};