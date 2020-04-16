#include<iostream>
#include "include/App.hpp"

#include<random>
#include <iomanip>
#include <QtWidgets>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    App app;
    app.setup();
    a.exec();
}