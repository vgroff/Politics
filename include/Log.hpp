#pragma once
#include<functional>
#include<string>

enum LogLevel {
    Verbose,
    Debug,
    Warn,
    Error
};

class Log {
private:
    static LogLevel logLevel;
    static std::function<void(std::string)> loggingFunction;
public:
    static void log(LogLevel logLevel, std::string logLine);
    static void log(std::string logLine);
    static void setLogger(std::function<void(std::string)> loggingFunction);
};