#include "LightController.hpp"
#include "Logger.hpp"
#include <sstream>

std::vector<LightController*> LightController::allLights;

LightController::LightController(const std::string& controllerName, int id)
    : Sensor(controllerName), 
      state(LightState::OFF),
      brightness(LightBrightness::OFF),
      roomId(id)
{
    allLights.push_back(this);
}

float LightController::readValue()
{
    std::lock_guard<std::mutex> lock(valueMutex);
    return static_cast<float>(static_cast<int>(brightness));
}

bool LightController::turnOn()
{
    std::lock_guard<std::mutex> lock(valueMutex);

    if (state == LightState::OFF)
    {
        state = LightState::ON;
        brightness = LightBrightness::MEDIUM;
        std::stringstream ss;
        ss << "Light in room " << roomId << " turned ON at " 
           << static_cast<int>(brightness) << "% brightness";
        Logger::getInstance() -> log(ss.str(), true);
        return true;
    }

    return false;
}

bool LightController::turnOff()
{
    std::lock_guard<std::mutex> lock(valueMutex);

    if (state == LightState::ON)
    {
        state = LightState::OFF;
        brightness = LightBrightness::OFF;

        std::stringstream ss;
        ss << "Light in room " << roomId << " turned OFF";
        Logger::getInstance() -> log(ss.str(), true);
        return true;
    }

    return false;
}

LightState LightController::getState() const
{
    return state;
}

LightBrightness LightController::getBrightness() const
{
    return brightness;
}

int LightController::getRoomId() const
{
    return roomId;
}

std::string LightController::getBrightnessName() const
{
    switch (brightness)
    {
        case LightBrightness::OFF:
            return "Off";
        case LightBrightness::LOW:
            return "Low";
        case LightBrightness::MEDIUM:
            return "Medium";
        case LightBrightness::HIGH:
            return "High";
        case LightBrightness::FULL:
            return "Full";
        default:
            return "Unknown";
    }
}

void LightController::turnOnAllLights()
{
    for (auto light : allLights)
    {
        light->turnOn();
    }
    
    Logger::getInstance() -> log("Command: Turning ON all lights", true);
}

void LightController::turnOffAllLights()
{
    for (auto light : allLights)
    {
        light->turnOff();
    }
    
    Logger::getInstance() -> log("Command: Turning OFF all lights", true);
}

bool LightController::setBrightness(LightBrightness level)
{
    std::lock_guard<std::mutex> lock(valueMutex);
    
    if (level == LightBrightness::OFF)
    {
        return turnOff();
    }
    
    if (state == LightState::OFF && level != LightBrightness::OFF)
    {
        state = LightState::ON;
    }
    
    if (brightness != level)
    {
        brightness = level;
        
        std::stringstream ss;
        ss << "Light in room " << roomId << " brightness set to " 
           << static_cast<int>(brightness) << "%";
        Logger::getInstance() -> log(ss.str(), true);
        return true;
    }
    
    return false;
}