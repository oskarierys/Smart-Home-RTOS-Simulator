#pragma once    

#include "TaskManager.hpp"
#include "Sensor.hpp"
#include <string>    
#include <memory>
class TemperatureSensorTask : public Task {
private:
    std::unique_ptr<TemperatureSensor> sensor;
    std::string name;
    int priority;
    std::chrono::steady_clock::time_point lastExecutionTime;

public:
    TemperatureSensorTask(const std::string& taskName, int taskPriority);
    void execute() override;
    const std::string& getName() const override;
    int getPriority() const override;    
};
