#include "../../include/gui/MainSection.hpp"

MainSection::MainSection(int minSizeHeight, int minSizeWidth, Engine* enginePtr, QWidget *parent)
    : QWidget(parent),
    enginePtr(enginePtr) {

    setMinimumSize(minSizeHeight, minSizeWidth);

    topLevelLayout = QPointer<QVBoxLayout>(new QVBoxLayout());

    windowsSection = QPointer<WindowsSection>(new WindowsSection(enginePtr));

    std::function<void(BottomSectionEvent)> eventHandler = std::bind(&MainSection::bottomSectionEventsHandler, this, std::placeholders::_1);
    bottomSection = QPointer<BottomSection>(new BottomSection(eventHandler));
    bottomSection->setMinimumSize(minSizeHeight, minSizeWidth/10);

    topLevelLayout->addWidget(windowsSection);
    topLevelLayout->addWidget(bottomSection);

    setLayout(topLevelLayout);
}

void MainSection::bottomSectionEventsHandler(BottomSectionEvent event) {
    if (event == nextTurnEvent) {
        enginePtr->playTurn();
    }
}
