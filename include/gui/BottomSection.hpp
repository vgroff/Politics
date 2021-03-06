#pragma once
#include "../Engine.hpp"
#include <QtWidgets>

enum BottomSectionEvent {
    nextTurnEvent,
    showNationEvent
};

class BottomSection : public QWidget {
    Q_OBJECT
private:
    QPointer<QHBoxLayout> topLevelLayout;
    QPointer<QPushButton> nextTurnButton;
    QPointer<QPushButton> showNationButton;
    std::function<void(BottomSectionEvent)> eventsHandler;
public:
    explicit BottomSection(std::function<void(BottomSectionEvent)> eventsHandler, QWidget *parent = nullptr);
};