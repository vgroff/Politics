#include<iostream>
#include "include/Engine.hpp"

int main() {
    Engine e = Engine::testSetup();
    for (int i = 0; i < 520; i++) {
        e.playTurn();
    }
    return 0;
}