#include "../../include/gui/WindowsSection.hpp"

WindowsSection::WindowsSection(Engine *enginePtr, QWidget *parent)
    : QWidget(parent),
    enginePtr(enginePtr) {

    setStyleSheet("background:blue;");
    windowButton = QPointer<QPushButton>(new QPushButton(this));

}

