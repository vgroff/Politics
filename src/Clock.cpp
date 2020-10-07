#include "../include/Clock.hpp"

Clock::Clock(size_t turnSizeDays, time_point startingDate) 
    : turnSizeDays(turnSizeDays), currentDate(startingDate) {}

void Clock::subscribe(std::function<void()> callback) {
    callbacks.push_back(callback);
}

void Clock::nextTurn() {
    currentDate += hours(24*turnSizeDays);
    for (auto callback : callbacks) {
        callback();
    }
}

time_point Clock::getCurrentDate() {
    return currentDate;
}

ClockSubscriber::ClockSubscriber(std::weak_ptr<Clock> clock) : clock(clock) {

}

ClockSubscriber::subscribeToClock() {
    std::weak_ptr<>();
}