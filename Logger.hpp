#pragma once

#include <string>
#include <mutex>
#include <fstream>
#include <iostream>

class Logger {
    private:
    static Logger* instance;
    std::mutex logMutex;
    std::ofstream logFile;
    bool consoleOutput;

    Logger();

public:
    static Logger* getInstance();
    void log(const std::string& message, bool toConsole = false);
    void setConsoleOutput(bool enabled);
    ~Logger();
};
