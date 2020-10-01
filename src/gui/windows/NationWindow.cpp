#include "../../../include/gui/windows/NationWindow.hpp"

NationWindow::NationWindow(Nation& nation, QWidget *parent)
    : QDialog(parent),
    nation(nation) {

    setWindowFlag(Qt::WindowStaysOnTopHint);
    setStyleSheet("background:blue;");
    nationName = QPointer<QLabel>(new QLabel(nation.name.c_str(), this));
    windowButton = QPointer<QPushButton>(new QPushButton("button", this));
    show();
}

