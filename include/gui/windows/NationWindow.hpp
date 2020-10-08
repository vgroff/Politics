#pragma once
#include "../../Nation.hpp"
#include "./Window.hpp"


class NationWindow : public Window {
    Q_OBJECT
private:
    std::weak_ptr<Nation> nationWeakPtr;
    QPointer<QVBoxLayout> topLevelLayout;
    QPointer<QLabel> nationName;
    QPointer<QPushButton> windowButton;

    void refreshWindow() override;

public:
    explicit NationWindow(std::weak_ptr<const Clock> clock, std::weak_ptr<Nation> nationWeakPtr, QWidget *parent = nullptr);
};