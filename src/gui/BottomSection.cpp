#include "../../include/gui/BottomSection.hpp"

BottomSection::BottomSection(std::function<void(BottomSectionEvent)> eventsHandler, QWidget *parent)
    : QWidget(parent), eventsHandler(eventsHandler) {

    setStyleSheet("background:blue;");

    topLevelLayout = QPointer<QHBoxLayout>(new QHBoxLayout());
    nextTurnButton = QPointer<QPushButton>(new QPushButton("Next turn"));
    topLevelLayout->addWidget(nextTurnButton);
    setLayout(topLevelLayout);

    auto self = this;
    QObject::connect(nextTurnButton, &QPushButton::clicked, [self]() {
        self->eventsHandler(nextTurnEvent);
    });
}

