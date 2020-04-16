#include<iostream>
#include "include/Engine.hpp"
#include<random>
#include <iomanip>

int main() {
    std::cout << std::setprecision(4);
    std::srand(38);
    Engine e = Engine::testSetup();
    for (int i = 0; i < 1; i++) {
        e.playTurn();
    }
    return 0;
}