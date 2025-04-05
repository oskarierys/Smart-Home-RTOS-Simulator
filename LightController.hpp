#pragma once

#include "Sensor.hpp"
#include <string>
#include <mutex>
#include <vector>
#include <memory>

enum class LightState
{
    OFF = 0,
    ON = 1
};

enum class LightBrightness
{
    OFF = 0,
    LOW = 25,
    MEDIUM = 50,
    HIGH = 75,
    FULL = 100
};

class LightController : public Sensor
{
private:
    LightState state;
    LightBrightness brightness;
    int roomId;
    static std::vector<LightController*> allLights;

public:
    explicit LightController(const std::string& controllerName, int id);
    float readValue() override;
    bool turnOn();
    bool turnOff();
    bool setBrightness(LightBrightness level);
    LightState getState() const;
    LightBrightness getBrightness() const;
    int getRoomId() const;
    std::string getBrightnessName() const;

    static void turnOnAllLights();
    static void turnOffAllLights();
};