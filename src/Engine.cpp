#include "../include/Engine.hpp"
#include "../include/Log.hpp"
#include <iostream>

Engine::Engine(time_point startingDate) 
    : clock(std::make_shared<Clock>(7, startingDate)) {
    playerNation->init();
}



void Engine::playTurn() {
    clock->nextTurn();
    std::ostringstream oss;
    oss << "Today is: " << clock->getCurrentDate();
    Log::log(oss.str());
}

std::shared_ptr<const Clock> Engine::getClock() { return clock; }

Engine Engine::testSetup() {
    Engine e(time_point(date(1997, 9, 20), hours(0)));
    return e;
};

// - So, main event loop goes:
//     - Industry produces. Then Pops consume. Then Capital re-invests. Taxes and subsidies are applied during this process. Population grows
//     - Could have electors and pops function entirely independently, where pops do the economics and electors are just attached to a certain pop
//     - How exactly will source of truth be maintained? Like if we have population attached to each industry, and to each nation, who controls it?