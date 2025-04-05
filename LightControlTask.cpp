#include "LightControlTask.hpp"
#include "Logger.hpp"
#include <random>
#include <ctime>
#include <sstream>

LightControlTask::LightControlTask(const std::string& taskName, int taskPriority)
    : name(taskName), priority(taskPriority)
{
    lastExecuted = std::chrono::steady_clock::now();

    controllers.push_back(std::make_unique<LightController>("Living Room Light", 1));
    controllers.push_back(std::make_unique<LightController>("Bedroom Light", 2));
    controllers.push_back(std::make_unique<LightController>("Kitchen Light", 3));
    controllers.push_back(std::make_unique<LightController>("Bathroom Light", 4));
    
    Logger::getInstance() -> log("Light control system initialized with 4 controllers", true);
}

void LightControlTask::execute()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastExecuted).count();

    if (elapsed >= 10)
    {
        applyTimeBasedRules();
        applyMotionBasedRules();

        std::stringstream ss;
        ss << "Light control status updated";
        Logger::getInstance() -> log(ss.str(), false);
        
        lastExecuted = now;
    }
}

float LightControlTask::simulateMotion(int roomId) const
{
    time_t now = time(0);
    struct tm timeInfo;
    
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    
    int hour = timeInfo.tm_hour;

    float baseChance = 0.0f;

    if (roomId == 1) 
    {
        if ((hour >= 7 && hour < 9) || (hour >= 18 && hour < 23)) 
        {
            baseChance = 70.0f;
        } 
        else if (hour >= 9 && hour < 18) 
        {
            baseChance = 30.0f;
        } 
        else 
        {
            baseChance = 5.0f; // Night
        }
    }
    // Bedroom (id=2): Active morning and night
    else if (roomId == 2) 
    {
        if ((hour >= 6 && hour < 8) || (hour >= 22 || hour < 1)) 
        {
            baseChance = 80.0f;
        } 
        else if (hour >= 1 && hour < 6) 
        {
            baseChance = 20.0f;
        } 
        else 
        {
            baseChance = 10.0f;
        }
    }

    else if (roomId == 3) 
    {
        if ((hour >= 7 && hour < 9) || (hour >= 12 && hour < 14) || (hour >= 18 && hour < 20)) 
        {
            baseChance = 90.0f;
        } 
        else 
        {
            baseChance = 20.0f;
        }
    }

    else if (roomId == 4) 
    {
        baseChance = 30.0f;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-20.0f, 20.0f);
    
    return std::max(0.0f, std::min(100.0f, baseChance + dis(gen)));
}

void LightControlTask::applyTimeBasedRules()
{
    time_t now = time(0);
    struct tm timeInfo;
    
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    
    int hour = timeInfo.tm_hour;

    if (hour >= 1 && hour < 6)
    {
        for (auto& controller : controllers)
        {
            if (controller -> getRoomId() != 2 && controller -> getState() == LightState::ON)
            {
                controller -> turnOff();
                std::stringstream ss;
                ss << "Late night rule: Turning off light in room " << controller->getRoomId();
                Logger::getInstance() -> log(ss.str(), true);
            }
        }
    }
    
    if (hour >= 18 && hour < 20)
    {
        for (auto& controller : controllers)
        {
            if (controller -> getRoomId() == 1 && controller -> getState() == LightState::OFF)
            {
                controller -> turnOn();
                std::stringstream ss;
                ss << "Evening rule: Turning on living room light";
                Logger::getInstance() -> log(ss.str(), true);
            }
        }
    }
}

void LightControlTask::applyMotionBasedRules()
{
    for (auto& controller : controllers)
    {
        int roomId = controller -> getRoomId();
        float motion = simulateMotion(roomId);
        
        if (motion > 75.0f && controller -> getState() == LightState::OFF)
        {
            controller -> turnOn();
            std::stringstream ss;
            ss << "Motion rule: Detected activity (" << motion 
               << "%) in room " << roomId << ", turning light on";
            Logger::getInstance() -> log(ss.str(), true);
        }
        else if (motion < 10.0f && controller->getState() == LightState::ON)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 100);
            
            if (dis(gen) <= 10)
            {
                controller -> turnOff();
                std::stringstream ss;
                ss << "Inactivity rule: No motion (" << motion 
                   << "%) in room " << roomId << ", turning light off";
                Logger::getInstance() -> log(ss.str(), true);
            }
        }
    }
}

const std::string& LightControlTask::getName() const
{
    return name;
}

int LightControlTask::getPriority() const
{
    return priority;
}

bool LightControlTask::setLight(int roomId, bool on)
{
    for (auto& controller : controllers)
    {
        if (controller -> getRoomId() == roomId)
        {
            return on ? controller -> turnOn() : controller -> turnOff();
        }
    }
    
    return false;
}

bool LightControlTask::setBrightness(int roomId, LightBrightness level)
{
    for (auto& controller : controllers)
    {
        if (controller -> getRoomId() == roomId)
        {
            return controller -> setBrightness(level);
        }
    }
    
    return false;
}

std::vector<std::pair<int, std::string>> LightControlTask::getStatusReport() const
{
    std::vector<std::pair<int, std::string>> report;
    
    for (const auto& controller : controllers)
    {
        std::stringstream ss;
        ss << "Room " << controller -> getRoomId() << ": " 
           << (controller -> getState() == LightState::ON ? "ON" : "OFF")
           << " (" << controller -> getBrightnessName() << ", " 
           << static_cast<int>(controller -> getBrightness()) << "%)";
        
        report.push_back({controller -> getRoomId(), ss.str()});
    }
    
    return report;
}