#pragma once

#include <string>
#include <mutex>
#include <fstream>

class Logger {
private:
    static Logger* instance;
    std::mutex logMutex;
    std::ofstream logFile;

    Logger();

public:
    static Logger* getInstance();
    void log(const std::string& message);
    ~Logger();
};