#include "../include/Engine.hpp"


Engine::Engine(date startingDate) 
: currentDate(startingDate) {
    
}

void Engine::playTurn() {
    playerNation.runIndustryTurn();
}

Engine Engine::testSetup() {
    Engine e(date(1997, 9, 20));
    return e;
};

// - So, main event loop goes:
//     - Industry produces. Then Pops consume. Then Capital re-invests. Taxes and subsidies are applied during this process.
//     - Could have electors and pops function entirely independently, where pops do the economics and electors are just attached to a certain pop
//     - How exactly will source of truth be maintained? Like if we have population attached to each industry, and to each nation, who controls it?