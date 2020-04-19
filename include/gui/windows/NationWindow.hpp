#pragma once
#include "../../Nation.hpp"
#include <QtWidgets>


class NationWindow : public QDialog {
    Q_OBJECT
private:
    Nation* nationPtr;
    QPointer<QPushButton> windowButton;

public:
    explicit NationWindow(Nation* nationPtr, QWidget *parent = nullptr);
};