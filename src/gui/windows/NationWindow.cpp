#include "../../../include/gui/windows/NationWindow.hpp"

NationWindow::NationWindow(std::weak_ptr<const Clock> clock, std::weak_ptr<Nation> nationWeakPtr, QWidget *parent)
    : Window(clock, parent),
    nationWeakPtr(nationWeakPtr) {

    auto nationPtr = nationWeakPtr.lock();
    auto nation = *nationPtr;

    setWindowFlag(Qt::WindowStaysOnTopHint);
    setStyleSheet("background:blue;");
    std::cout << nation.name << std::endl;
    topLevelLayout = QPointer<QVBoxLayout>(new QVBoxLayout());

    nationName = QPointer<QLabel>(new QLabel(nation.name.c_str(), this));
    windowButton = QPointer<QPushButton>(new QPushButton("button", this));

    topLevelLayout->addWidget(nationName);
    topLevelLayout->addWidget(windowButton);
    setLayout(topLevelLayout);
    show();
}

void NationWindow::refreshWindow() {
    nationName->setText("refreshed");
}