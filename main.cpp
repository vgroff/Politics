#include<iostream>
#include "include/Engine.hpp"
#include<random>
#include <iomanip>
#include <QtWidgets>

int main(int argc, char *argv[]) {
    std::cout << std::setprecision(4);
    std::srand(38);
    Engine e = Engine::testSetup();
    for (int i = 0; i < 1; i++) {
        e.playTurn();
    }
    // Creates an instance of QApplication
    QApplication a(argc, argv);

    QWidget window;
    window.resize(320, 240);
    window.show();
    window.setWindowTitle(
        QApplication::translate("toplevel", "Top-level widget"));

    // run the application and return execs() return value/code
    return a.exec();
}