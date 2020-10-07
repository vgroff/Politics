#pragma once

#include "common/Date.hpp"
#include "Nation.hpp"

class Engine {
private:
    time_point currentDate;
    
public:
    Engine(time_point startingDate);
    void playTurn();
    std::shared_ptr<Nation> playerNation = std::make_shared<Nation>(Nation::testSetupSingleNation());

    static Engine testSetup();
};