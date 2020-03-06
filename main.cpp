#include<iostream>
#include "include/Engine.hpp"
#include<random>

int main() {
    std::srand(38);
    Engine e = Engine::testSetup();
    for (int i = 0; i < 3; i++) {
        e.playTurn();
    }
    return 0;
}