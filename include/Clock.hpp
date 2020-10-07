#pragma once

#include "common/Date.hpp"

class Clock {
friend class Engine;
private: 
    time_point currentDate;
    size_t turnSizeDays;

    std::vector<std::function<void()>> callbacks;

    void nextTurn();
public:
    Clock(size_t turnSizeDays, time_point startingDate);
    void subscribe(std::function<void()> callback);

    time_point getCurrentDate();
    size_t getTurnSizeDays();
};

class ClockSubscriber {
private:
    std::weak_ptr<Clock> clock;
    void subscribeToClock(std::function<void()> callback);
public:
    ClockSubscriber(std::weak_ptr<Clock> clock);
}