#include "TaskManager.hpp"
#include "Logger.hpp"
#include "EventManager.hpp"
#include "TemperatureSensorTask.hpp" 
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <functional>
#include <atomic>

class ControlPanel
{
private:
    std::atomic<bool> running{true};
    std::map<std::string, std::function<void()>> commands;
    TaskManager* taskManager;
    std::chrono::steady_clock::time_point startTime;
public:
    ControlPanel(TaskManager* tm) :taskManager(tm)
    {
        startTime = std::chrono::steady_clock::now();

        commands["help"] = [this]() { showHelp(); };
        commands["quit"] = [this]() 
        {
            running = false;
            std::cout << "Shutting down the Smart Home RTOS Simulator...\n";
            taskManager -> stopScheduler();
            Logger::getInstance() -> log("Shutdown command received");
            std::cout << "Goodbye!\n";
        };
        commands["stats"] = [this]() { showStats(); };
        commands["temp"] = [this]() { showTemperature(); };
    }

    void showHelp()
    {
        std::cout << "\n=== Smart Home RTOS Simulator Control Panel ===\n";
        std::cout << "Available commands:\n";
        std::cout << "  help   - Show this help message\n";
        std::cout << "  quit   - Exit the application\n";
        std::cout << "  stats  - Show system statistics\n";
        std::cout << "  temp   - Show current temperature\n";
        std::cout << "============================================\n";
    }

    void showStats()
    {
        auto stats = taskManager -> getStatistics();

        std::cout << "\n=== System Statistics ===\n";
        std::cout << "Total tasks: " << stats.totalTasks << "\n";
        std::cout << "Active tasks: " << stats.activeTasks << "\n";
        std::cout << "System uptime: " << getUpTime() << " seconds\n";

        std::cout << "\n === Task Priorities ===\n";
        for (const auto& [name, priority] : stats.taskPriorities)
        {
            std::cout << "Task: " << name << ", Priority: " << priority << "\n";
        }
        std::cout << "========================\n";
    }

    void showTemperature()
    {
        float temp = TemperatureSensor::getLastReading();

        std::cout << "\n=== Current Temperature ===\n";
        std::cout << "Reading latest temperature...\n";
        std::cout << "Temperature: " << temp << " C\n";

        if (temp > 25.0)
        {
            std::cout << "Warning: High temperature detected!\n";
        }
        else if (temp < 20.0)
        {
            std::cout << "Warning: Low temperature detected!\n";
        }
        else
        {
            std::cout << "Temperature is within normal range.\n";
        }

        std::cout << "===========================\n";
    }

    int getUpTime()
    {
        auto currentTime = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
    }

    void run()
    {
        showHelp();

        while (running)
        {
            std::cout << "\nEnter command (or 'help' for available commands): ";
            std::string input;
            std::getline(std::cin, input);

            if (commands.find(input) != commands.end()) 
            {
                commands[input]();
            } 
            else 
            {
                std::cout << "Unknown command: " << input << "\n";
                std::cout << "Type 'help' for available commands\n";
            }
        }
    }

    bool isRunning() const
    {
        return running;
    }
};

int main() {
    auto logger = Logger::getInstance();
    logger -> log("Application started");

    auto taskManager = TaskManager::getInstance();
    taskManager -> addTask(std::make_unique<TemperatureSensorTask>("Temperature Sensor Task", 1));
    taskManager -> startScheduler();

    ControlPanel controlPanel(taskManager);
    controlPanel.run();

    logger -> log("Application stopped");

    return 0;
}