#include "TemperatureSensorTask.hpp"
#include "Logger.hpp"
#include "Sensor.hpp"
   
constexpr char TempSensor1[] = "Sensor1";

TemperatureSensorTask::TemperatureSensorTask(const std::string& taskName, int taskPriority)
    : name(taskName), priority(taskPriority)
{
    sensor = std::make_unique<TemperatureSensor>(TempSensor1);
}

void TemperatureSensorTask::execute()
{
    float temperature = sensor -> readValue();
    Logger::getInstance() -> log("Temperature Sensor Task: " + name + " - Temperature: " + std::to_string(temperature) + "°C");
}

const std::string& TemperatureSensorTask::getName() const
{
    return name;
}

int TemperatureSensorTask::getPriority() const
{
    return priority;
}