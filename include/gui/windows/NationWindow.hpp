#pragma once
#include "../../Nation.hpp"
#include <QtWidgets>


class NationWindow : public QDialog {
    Q_OBJECT
private:
    Nation& nation;
    QPointer<QLabel> nationName;
    QPointer<QPushButton> windowButton;

public:
    explicit NationWindow(Nation& nation, QWidget *parent = nullptr);
};