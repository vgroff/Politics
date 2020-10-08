#include "../../include/gui/WindowsSection.hpp"
#include "../../include/gui/windows/NationWindow.hpp"


WindowsSection::WindowsSection(Engine *enginePtr, QWidget *parent)
    : QWidget(parent),
    enginePtr(enginePtr) {

    setStyleSheet("background:blue;");
    windowButton = QPointer<QPushButton>(new QPushButton("button", window));
}

void WindowsSection::showNation() {
    window = QPointer<NationWindow>(new NationWindow(enginePtr->getClock(), enginePtr->playerNation, this));
    window->init();
    window->show();
}