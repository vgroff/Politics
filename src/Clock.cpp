#include "../include/Clock.hpp"
#include "../include/Log.hpp"

/********** Clock methods ****************/
Clock::Clock(size_t turnSizeDays, time_point startingDate) 
    : turnSizeDays(turnSizeDays), currentDate(startingDate) {}

size_t Clock::subscribe(std::function<void(time_point, size_t)> callback) const {
    id += 1;
    callbacks[id] = callback;
    return id;
}

void Clock::unsubscribe(size_t id) const {
    callbacks.erase(id);
}

void Clock::nextTurn() {
    currentDate += hours(24*turnSizeDays);
    for (auto [id, callback] : callbacks) {
        callback(currentDate, turnSizeDays);
    }
}

time_point Clock::getCurrentDate() const {
    return currentDate;
}


/********** ClockSubscriber methods ****************/
class NoClockException : public std::exception
{
	const char * what () const throw ()
    {
    	return "No Clock Exception";
    }
};

ClockSubscriber::ClockSubscriber(std::weak_ptr<const Clock> clock) : weakClock(clock) {

}

void ClockSubscriber::subscribeToClock(std::function<void(time_point, size_t)> callback) {
    auto clock = weakClock.lock();
    if (clock) {
        size_t id = clock->subscribe(callback);
        callbackIds.push_back(id);
    } else {
        Log::log(Error, "No clock detected");
        throw NoClockException();
    }
}

ClockSubscriber::~ClockSubscriber() {
    auto clock = weakClock.lock();
    if (clock) {
        for (auto callbackId : callbackIds) {
            clock->unsubscribe(callbackId);
        }
    }
}