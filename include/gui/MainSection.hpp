#pragma once
#include <QtWidgets>
#include "../Engine.hpp"
#include "WindowsSection.hpp"
#include "BottomSection.hpp"


class MainSection : public QWidget {
    Q_OBJECT
private:
    Engine* enginePtr;
    QPointer<QVBoxLayout> topLevelLayout;
    QPointer<WindowsSection> windowsSection;
    QPointer<QWidget> bottomSection;

    void bottomSectionEventsHandler(BottomSectionEvent event);
public:
    explicit MainSection( int minSizeWidth, int minSizeHeight, Engine *enginePtr, QWidget *parent = nullptr);
};