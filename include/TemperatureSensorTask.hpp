#pragma once

#include "TaskManager.hpp"
#include "Sensor.hpp"
#include <string>
#include <mutex>
#include <random>
#include <chrono>

class TemperatureSensor : public Sensor {
private:
    static float currentTemperature;
    static std::mutex temperatureMutex;
    std::mt19937 rng;
    std::uniform_real_distribution<float> tempVariation;

public:
    explicit TemperatureSensor(const std::string& sensorName);
    float readValue() override;
    
    static float getLastReading();
    static void setReading(float temperature);
};

class TemperatureSensorTask : public Task {
private:
    std::string name;
    int priority;
    std::unique_ptr<TemperatureSensor> sensor;
    std::chrono::steady_clock::time_point lastExecuted;

    float simulateTemperature() const;

public:
    TemperatureSensorTask(const std::string& taskName, int taskPriority);
    void execute() override;
    const std::string& getName() const override;
    int getPriority() const override;
};