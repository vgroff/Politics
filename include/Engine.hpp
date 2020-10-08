#pragma once

#include "Clock.hpp"
#include "Nation.hpp"

class Engine {
private:
    std::shared_ptr<Clock> clock;

    bool initialised = false;
public:
    Engine(time_point startingDate);
    void playTurn();
    void initialise();
    std::shared_ptr<Nation> playerNation = std::make_shared<Nation>(Nation::testSetupSingleNation(clock));
    
    std::shared_ptr<const Clock> getClock();
    static Engine testSetup();
};