#pragma once

#include "common/Date.hpp"

class Clock {
friend class Engine;
private: 
    time_point currentDate;
    size_t turnSizeDays;
    mutable size_t id = 1;

    mutable std::map<size_t, std::function<void(time_point, size_t)>> callbacks;

    void nextTurn();
public:
    Clock(size_t turnSizeDays, time_point startingDate);
    size_t subscribe(std::function<void(time_point, size_t)> callback) const;
    void unsubscribe(size_t id) const;

    time_point getCurrentDate() const;
    size_t getTurnSizeDays() const;
};

class ClockSubscriber {
protected:
    std::vector<size_t> callbackIds;
    std::weak_ptr<const Clock> weakClock;
    void subscribeToClock(std::function<void(time_point, size_t)> callback);
public:
    ClockSubscriber(std::weak_ptr<const Clock> clock);
    ~ClockSubscriber();
};