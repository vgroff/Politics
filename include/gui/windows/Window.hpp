#pragma once
#include "../../Nation.hpp"
#include <QtWidgets>


class Window : public QDialog {
    Q_OBJECT
private:
    virtual void refreshWindow() = 0;

public:
    explicit Window(QWidget *parent = nullptr);
};