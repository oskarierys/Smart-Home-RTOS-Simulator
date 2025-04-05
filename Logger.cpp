#include "Logger.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>

Logger* Logger::instance = nullptr;

Logger::Logger() : consoleOutput(false)
{
    logFile.open("system.log", std::ios::app);
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

Logger* Logger::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Logger();
    }

    return instance;
}

void Logger::log(const std::string& message, bool toConsole)
{
    std::lock_guard<std::mutex> lock(logMutex);

    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;
    
#ifdef _WIN32
    localtime_s(&timeInfo, &timeT);
#else
    localtime_r(&timeT, &timeInfo);
#endif
    
    char timeBuffer[25];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    
    std::string logEntry = "[" + std::string(timeBuffer) + "] " + message;
    
    if (logFile.is_open())
    {
        logFile << logEntry << std::endl;
        logFile.flush();
    }
    
    if (toConsole || consoleOutput)
    {
        std::cout << logEntry << std::endl;
    }
}

void Logger::setConsoleOutput(bool enabled)
{
    consoleOutput = enabled;
}