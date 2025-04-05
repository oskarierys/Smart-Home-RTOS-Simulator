#include "Logger.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>

Logger* Logger::instance = nullptr;

Logger::Logger()
{
    logFile.open("smart_home.log", std::ios::out | std::ios::app);
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
    delete instance;
    instance = nullptr;
}

Logger* Logger::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Logger();
    }

    return instance;
}

void Logger::log(const std::string& message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm_buf;
    localtime_s(&tm_buf, &time);

    std::string timeStr = std::string("[") +
                            std::to_string(tm_buf.tm_year + 1900) + "-" +
                            std::to_string(tm_buf.tm_mon + 1) + "-" +
                            std::to_string(tm_buf.tm_mday) + " " +
                            std::to_string(tm_buf.tm_hour) + ":" +
                            std::to_string(tm_buf.tm_min) + ":" +
                            std::to_string(tm_buf.tm_sec) + "] ";

    std::cout << timeStr << message << std::endl;
    
    if (logFile.is_open())
    {
        logFile << timeStr << message << std::endl;
    }
}
