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
#include "WindowBlindTask.hpp"
#include "LightControlTask.hpp"

class ControlPanel
{
private:
    std::atomic<bool> running{true};
    std::map<std::string, std::function<void()>> commands;
    TaskManager* taskManager;
    std::chrono::steady_clock::time_point startTime;
    WindowBlindTask* blindsTask;
    LightControlTask* lightTask;

public:
    ControlPanel(TaskManager* tm, WindowBlindTask* bTask, LightControlTask* lTask) 
        : taskManager(tm), blindsTask(bTask), lightTask(lTask)
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
        commands["blinds"] = [this]() { showBlindOptions(); };
        commands["lights"] = [this]() { showLightOptions(); };
        commands["status"] = [this]() { showSystemStatus(); };
    }

    void showHelp()
    {
        std::cout << "\n=== Smart Home RTOS Simulator Control Panel ===\n";
        std::cout << "Available commands:\n";
        std::cout << "  help   - Show this help message\n";
        std::cout << "  quit   - Exit the application\n";
        std::cout << "  stats  - Show system statistics\n";
        std::cout << "  temp   - Show current temperature\n";
        std::cout << "  blinds - Control window blinds\n";
        std::cout << "  lights - Control room lights\n";
        std::cout << "  status - Show complete system status\n";
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

    void showBlindOptions()
    {
        std::cout << "\n=== Window Blinds Control ===\n";
        std::cout << "Current blinds status:\n";
        
        auto status = blindsTask->getStatusReport();
        for (const auto& [id, statusMsg] : status)
        {
            std::cout << statusMsg << "\n";
        }
        
        std::cout << "\nOptions:\n";
        std::cout << "1. Set position for specific window\n";
        std::cout << "2. Open all blinds\n";
        std::cout << "3. Close all blinds\n";
        std::cout << "0. Return to main menu\n";
        
        int choice;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1)
        {
            int windowId;
            int position;
            
            std::cout << "Enter window ID (1-3): ";
            std::cin >> windowId;
            
            std::cout << "Enter position (0=Closed, 25=Quarter, 50=Half, 75=ThreeQuarters, 100=Open): ";
            std::cin >> position;
            std::cin.ignore();
            
            BlindsPosition pos;
            switch (position)
            {
                case 0: pos = BlindsPosition::CLOSED; break;
                case 25: pos = BlindsPosition::QUARTER_OPEN; break;
                case 50: pos = BlindsPosition::HALF_OPEN; break;
                case 75: pos = BlindsPosition::THREE_QUARTERS_OPEN; break;
                case 100: pos = BlindsPosition::OPEN; break;
                default: 
                    std::cout << "Invalid position! Using default (HALF_OPEN).\n";
                    pos = BlindsPosition::HALF_OPEN;
            }
            
            if (blindsTask->setBlindsPosition(windowId, pos))
            {
                std::cout << "Window " << windowId << " blinds position updated.\n";
            }
            else
            {
                std::cout << "Failed to update position. Check window ID or wait for cooldown.\n";
            }
        }
        else if (choice == 2)
        {
            WindowBlindController::openAllBlinds();
            std::cout << "Command sent to open all blinds.\n";
        }
        else if (choice == 3)
        {
            WindowBlindController::closeAllBlinds();
            std::cout << "Command sent to close all blinds.\n";
        }
        
        std::cout << "===========================\n";
    }

    void showLightOptions()
    {
        std::cout << "\n=== Light Control ===\n";
        std::cout << "Current light status:\n";
        
        auto status = lightTask->getStatusReport();
        for (const auto& [id, statusMsg] : status)
        {
            std::cout << statusMsg << "\n";
        }
        
        std::cout << "\nOptions:\n";
        std::cout << "1. Turn on/off specific room light\n";
        std::cout << "2. Set brightness for specific room\n";
        std::cout << "3. Turn on all lights\n";
        std::cout << "4. Turn off all lights\n";
        std::cout << "0. Return to main menu\n";
        
        int choice;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1)
        {
            int roomId;
            int state;
            
            std::cout << "Enter room ID (1-4): ";
            std::cin >> roomId;
            
            std::cout << "Set light state (0=Off, 1=On): ";
            std::cin >> state;
            std::cin.ignore();
            
            if (lightTask->setLight(roomId, state != 0))
            {
                std::cout << "Room " << roomId << " light updated.\n";
            }
            else
            {
                std::cout << "Failed to update light. Check room ID or current state.\n";
            }
        }
        else if (choice == 2)
        {
            int roomId;
            int brightness;
            
            std::cout << "Enter room ID (1-4): ";
            std::cin >> roomId;
            
            std::cout << "Set brightness (0=Off, 25=Low, 50=Medium, 75=High, 100=Full): ";
            std::cin >> brightness;
            std::cin.ignore();
            
            LightBrightness level;
            switch (brightness)
            {
                case 0: level = LightBrightness::OFF; break;
                case 25: level = LightBrightness::LOW; break;
                case 50: level = LightBrightness::MEDIUM; break;
                case 75: level = LightBrightness::HIGH; break;
                case 100: level = LightBrightness::FULL; break;
                default: 
                    std::cout << "Invalid brightness! Using default (MEDIUM).\n";
                    level = LightBrightness::MEDIUM;
            }
            
            if (lightTask->setBrightness(roomId, level))
            {
                std::cout << "Room " << roomId << " brightness updated.\n";
            }
            else
            {
                std::cout << "Failed to update brightness. Check room ID.\n";
            }
        }
        else if (choice == 3)
        {
            LightController::turnOnAllLights();
            std::cout << "Command sent to turn on all lights.\n";
        }
        else if (choice == 4)
        {
            LightController::turnOffAllLights();
            std::cout << "Command sent to turn off all lights.\n";
        }
        
        std::cout << "=====================\n";
    }

    void showSystemStatus()
    {
        std::cout << "\n=== Smart Home System Status ===\n";
        
        // Temperature
        float temp = TemperatureSensor::getLastReading();
        std::cout << "Temperature: " << temp << " C\n";
        
        // Uptime
        std::cout << "System uptime: " << getUpTime() << " seconds\n";
        
        // Lights status
        std::cout << "\nLights Status:\n";
        auto lightStatus = lightTask->getStatusReport();
        for (const auto& [id, statusMsg] : lightStatus)
        {
            std::cout << "  " << statusMsg << "\n";
        }
        
        // Blinds status
        std::cout << "\nWindow Blinds Status:\n";
        auto blindsStatus = blindsTask->getStatusReport();
        for (const auto& [id, statusMsg] : blindsStatus)
        {
            std::cout << "  " << statusMsg << "\n";
        }
        
        std::cout << "=============================\n";
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

    logger -> setConsoleOutput(false);
    logger -> log("Application started", true);

    auto taskManager = TaskManager::getInstance();

    auto temperatureSensorTask = std::make_unique<TemperatureSensorTask>("Temperature Sensor Task", 1);
    
    auto windowBlindTaskPtr = std::make_unique<WindowBlindTask>("Window Blind Control Task", 2);
    WindowBlindTask* windowBlindTaskRawPtr = windowBlindTaskPtr.get();
    
    auto lightControlTaskPtr = std::make_unique<LightControlTask>("Light Control Task", 3);
    LightControlTask* lightControlTaskRawPtr = lightControlTaskPtr.get();

    taskManager -> addTask(std::move(temperatureSensorTask));
    taskManager -> addTask(std::move(windowBlindTaskPtr));
    taskManager -> addTask(std::move(lightControlTaskPtr));

    taskManager -> startScheduler();

    ControlPanel controlPanel(taskManager, windowBlindTaskRawPtr, lightControlTaskRawPtr);
    controlPanel.run();

    logger -> log("Application stopped", true);

    return 0;
}