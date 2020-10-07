#include "../include/Engine.hpp"
#include "../include/Log.hpp"
#include <iostream>

Engine::Engine(time_point startingDate) 
    : currentDate(startingDate), clock(std::make_shared<Clock>(7, startingDate)) {
    
}

void Engine::playTurn() {
    clock->nextTurn();
    std::ostringstream oss;
    oss << "Today is: " << currentDate;
    Log::log(oss.str());
    playerNation->runIndustryTurn();
}

std::shared_ptr<Clock> Engine::getClock() { return clock; }

Engine Engine::testSetup() {
    Engine e(time_point(date(1997, 9, 20), hours(0)));
    return e;
};

// - So, main event loop goes:
//     - Industry produces. Then Pops consume. Then Capital re-invests. Taxes and subsidies are applied during this process. Population grows
//     - Could have electors and pops function entirely independently, where pops do the economics and electors are just attached to a certain pop
//     - How exactly will source of truth be maintained? Like if we have population attached to each industry, and to each nation, who controls it?