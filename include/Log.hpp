#pragma once
#include<functional>
#include<string>

class Log {
private:
    static std::function<void(std::string)> loggingFunction;
public:
    static void log(std::string logLine);
    static void setLogger(std::function<void(std::string)> loggingFunction);
};