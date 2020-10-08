#pragma once
#include "../../Clock.hpp"
#include <QtWidgets>


class Window : public QDialog, ClockSubscriber {
    Q_OBJECT
private:
    virtual void refreshWindow() = 0;

public:
    explicit Window(std::weak_ptr<const Clock> clock, QWidget *parent = nullptr);
    void init();
};