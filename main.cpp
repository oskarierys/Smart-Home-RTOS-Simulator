#include "TaskManager.hpp"
#include "Logger.hpp"
#include "EventManager.hpp"
#include "TemperatureSensorTask.hpp" 
#include <iostream>

int main() {
    auto logger = Logger::getInstance();
    logger -> log("Application started");

    auto taskManager = TaskManager::getInstance();
    taskManager -> addTask(std::make_unique<TemperatureSensorTask>("Temperature Sensor Task", 1));
    taskManager -> startScheduler();

    std::string inputCommand;
    while (std::getline(std::cin, inputCommand)) {
        if (inputCommand == "exit") {
            break;
        }
    }

    taskManager -> stopScheduler();
    logger -> log("Application stopped");

    return 0;
}