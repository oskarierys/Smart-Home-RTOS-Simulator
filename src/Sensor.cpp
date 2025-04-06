#include "Sensor.hpp"
#include <random>

float TemperatureSensor::lastReading = 0.0f;

Sensor::Sensor(const std::string& sensorName) : name(sensorName), currentValue(0.0f) {}

const std::string& Sensor::getName() const
{
    return name;
}

TemperatureSensor::TemperatureSensor(const std::string& sensorName) : Sensor(sensorName) {}

float TemperatureSensor::readValue()
{
    std::lock_guard<std::mutex> lock(valueMutex);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(20.0f, 30.0f);

    currentValue = dis(gen);
    lastReading = currentValue;
    return currentValue;
}

float TemperatureSensor::getLastReading()
{
    return lastReading;
}

