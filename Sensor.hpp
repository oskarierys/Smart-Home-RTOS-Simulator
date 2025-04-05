#pragma once

#include <string>
#include <mutex>

class Sensor {
protected:
    std::string name;
    float currentValue;
    std::mutex valueMutex;

public:
    explicit Sensor(const std::string& sensorName);
    virtual ~Sensor() = default;    
    virtual float readValue() = 0;
    const std::string& getName() const;
};

class TemperatureSensor : public Sensor {
private:
    static float lastReading;
public:
    explicit TemperatureSensor(const std::string& sensorName);
    float readValue() override;
    static float getLastReading();
};