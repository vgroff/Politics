#include<iostream>
#include "include/Engine.hpp"

int main() {
    Engine e = Engine::testSetup();
    e.playTurn();
    return 0;
}