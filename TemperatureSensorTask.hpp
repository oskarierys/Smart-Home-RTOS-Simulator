#pragma once 

#include "TaskManager.hpp"
#include "Sensor.hpp"

class TemperatureSensorTask : public Task {
private:
    std::unique_ptr<TemperatureSensor> sensor;
    std::string name;
    int priority;

public:
    TemperatureSensorTask(const std::string taskName, int taskPriority);
    void execute() override;
    const std::string& getName() const override;
    int getPriority() const override;    
};
