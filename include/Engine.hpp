#pragma once

#include "Clock.hpp"
#include "Nation.hpp"

class Engine {
private:
    time_point currentDate;
    std::shared_ptr<Clock> clock;
public:
    Engine(time_point startingDate);
    void playTurn();
    std::shared_ptr<Nation> playerNation = std::make_shared<Nation>(Nation::testSetupSingleNation(clock));
    
    std::shared_ptr<Clock> getClock();
    static Engine testSetup();
};