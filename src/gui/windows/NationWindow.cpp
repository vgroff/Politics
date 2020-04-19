#include "../../../include/gui/windows/NationWindow.hpp"

NationWindow::NationWindow(Nation *nationPtr, QWidget *parent)
    : QDialog(parent),
    nationPtr(nationPtr) {

    setWindowFlag(Qt::WindowStaysOnTopHint);
    setStyleSheet("background:blue;");
    windowButton = QPointer<QPushButton>(new QPushButton("button", this));
    show();
}

