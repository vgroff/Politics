#include "../../../include/gui/windows/Window.hpp"

Window::Window(std::weak_ptr<const Clock> clock, QWidget *parent): ClockSubscriber(clock), QDialog(parent) {
}

void Window::init() {
    subscribeToClock([this](time_point currentDate, size_t num) {
        this->refreshWindow();
    });
}