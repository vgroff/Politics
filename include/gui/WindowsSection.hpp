#pragma once
#include "../Engine.hpp"
#include <QtWidgets>


class WindowsSection : public QWidget {
    Q_OBJECT
private:
    Engine* enginePtr;
    QPointer<QPushButton> windowButton;
    QPointer<QDialog> window;

public:
    explicit WindowsSection(Engine *enginePtr, QWidget *parent = nullptr);

    void showNation();
};