#include "../include/Log.hpp"

LogLevel Log::logLevel = Verbose; 
std::function<void(std::string)> Log::loggingFunction = nullptr;

void Log::log(LogLevel logLevel_, std::string logLine) {
    if (loggingFunction) {
        if (logLevel_ > logLevel) {
            Log::loggingFunction(logLine);
        }
    }
}

void Log::log(std::string logLine) {
    if (loggingFunction) {
        Log::loggingFunction(logLine);
    }
}

void Log::setLogger(std::function<void(std::string)> loggingFunction_) {
    Log::loggingFunction = loggingFunction_;
}