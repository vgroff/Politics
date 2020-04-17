#include "../include/Log.hpp"

std::function<void(std::string)> Log::loggingFunction = nullptr;

void Log::log(std::string logLine) {
    if (loggingFunction) {
        Log::loggingFunction(logLine);
    }
}

void Log::setLogger(std::function<void(std::string)> loggingFunction_) {
    Log::loggingFunction = loggingFunction_;
}