#include "TemperatureSensorTask.hpp"
#include "Logger.hpp"
#include "Sensor.hpp"
#include <thread>
#include <chrono>
   
constexpr char TempSensor1[] = "Sensor1";

TemperatureSensorTask::TemperatureSensorTask(const std::string& taskName, int taskPriority)
    : name(taskName), priority(taskPriority)
{
    sensor = std::make_unique<TemperatureSensor>(TempSensor1);
    lastExecutionTime = std::chrono::steady_clock::now();
}

void TemperatureSensorTask::execute()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastExecutionTime).count();
    
    if (elapsed >= 20)
    {
        float temperature = sensor->readValue();
        Logger::getInstance()->log("Temperature Sensor Task: " + name + " - Temperature: " + std::to_string(temperature) + "°C", false);
        lastExecutionTime = now;
        
        isReady = false;
        
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            isReady = true;
        }).detach();
    }
}

const std::string& TemperatureSensorTask::getName() const
{
    return name;
}

int TemperatureSensorTask::getPriority() const
{
    return priority;
}