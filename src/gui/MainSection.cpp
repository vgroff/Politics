#include "../../include/gui/MainSection.hpp"

MainSection::MainSection(int minSizeWidth, int minSizeHeight,Engine* enginePtr, QWidget *parent)
    : QWidget(parent),
    enginePtr(enginePtr) {

    setMinimumSize(minSizeWidth, minSizeHeight);

    topLevelLayout = QPointer<QVBoxLayout>(new QVBoxLayout());

    windowsSection = QPointer<WindowsSection>(new WindowsSection(enginePtr, this));
    windowsSection->setMinimumSize(minSizeWidth, minSizeHeight*8/10);

    std::function<void(BottomSectionEvent)> eventHandler = std::bind(&MainSection::bottomSectionEventsHandler, this, std::placeholders::_1);
    bottomSection = QPointer<BottomSection>(new BottomSection(eventHandler, this));
    bottomSection->setMinimumSize(minSizeWidth, minSizeHeight/10);

    topLevelLayout->setAlignment(Qt::AlignHCenter);
    topLevelLayout->setContentsMargins(0,0,0,0);
    topLevelLayout->addWidget(windowsSection);
    topLevelLayout->addWidget(bottomSection);

    setLayout(topLevelLayout);
}

void MainSection::bottomSectionEventsHandler(BottomSectionEvent event) {
    if (event == nextTurnEvent) {
        enginePtr->playTurn();
    } else if (event == showNationEvent) {
        windowsSection->showNation();
    }
}
