#include "WindowBlindTask.hpp"
#include "Logger.hpp"
#include <random>
#include <ctime>
#include <sstream>

WindowBlindTask::WindowBlindTask(const std::string& taskName, int taskPriority)
    : name(taskName), priority(taskPriority)
{
    lastExecuted = std::chrono::steady_clock::now();

    controllers.push_back(std::make_unique<WindowBlindController>("Living Room Blinds", 1));
    controllers.push_back(std::make_unique<WindowBlindController>("Bedroom Blinds", 2));
    controllers.push_back(std::make_unique<WindowBlindController>("Kitchen Blinds", 3));
    
    Logger::getInstance() -> log("Window blinds system initialized with 3 controllers", true);
}

void WindowBlindTask::execute()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastExecuted).count();

    if (elapsed >= 15)
    {
        float lightLevel = simulateOutdoorLight();

        applyTimeBasedRules();
        applyLightBasedRules(lightLevel);

        std::stringstream ss;
        ss << "Window blinds status update - Light level: " << lightLevel;
        Logger::getInstance() -> log(ss.str(), false);
        
        lastExecuted = now;
    }
}

float WindowBlindTask::simulateOutdoorLight() const
{
    time_t now = time(0);
    struct tm timeInfo;
    
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    
    int hour = timeInfo.tm_hour;

    float baseLight = 0.0f;
    
    if (hour >= 7 && hour < 10) 
    {
        baseLight = 40.0f + (hour - 7) * 20.0f;
    } 
    else if (hour >= 10 && hour < 16) 
    {
        baseLight = 80.0f;
    }
    else if (hour >= 16 && hour < 20) 
    {
        baseLight = 80.0f - (hour - 16) * 20.0f;
    }
    else 
    {
        baseLight = 5.0f;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-10.0f, 10.0f);
    
    return std::max(0.0f, std::min(100.0f, baseLight + dis(gen)));
}

void WindowBlindTask::applyTimeBasedRules()
{
    time_t now = time(0);
    struct tm timeInfo;
    
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    
    int hour = timeInfo.tm_hour;

    if (hour >= 7 && hour <= 9)
    {
        for (auto& controller : controllers)
        {
            if (controller -> getPosition() == BlindsPosition::CLOSED)
            {
                controller -> setPosition(BlindsPosition::HALF_OPEN);
                std::stringstream ss;
                ss << "Morning rule: Opening blinds for window " << controller->getWindowId();
                Logger::getInstance() -> log(ss.str(), true);
            }
        }
    }
    
    if (hour >= 21 || hour < 6)
    {
        for (auto& controller : controllers)
        {
            if (controller -> getPosition() != BlindsPosition::CLOSED)
            {
                controller -> setPosition(BlindsPosition::CLOSED);
                std::stringstream ss;
                ss << "Night rule: Closing blinds for window " << controller->getWindowId();
                Logger::getInstance() -> log(ss.str(), true);
            }
        }
    }
}

void WindowBlindTask::applyLightBasedRules(float lightLevel)
{
    if (lightLevel > 90.0f)
    {
        for (auto& controller : controllers)
        {
            if (controller -> getPosition() == BlindsPosition::OPEN)
            {
                controller -> setPosition(BlindsPosition::THREE_QUARTERS_OPEN);
                std::stringstream ss;
                ss << "High light rule: Adjusting blinds for window " << controller -> getWindowId() 
                   << " due to bright light (" << lightLevel << "%)";
                Logger::getInstance() -> log(ss.str(), true);
            }
        }
    }
}

const std::string& WindowBlindTask::getName() const
{
    return name;
}

int WindowBlindTask::getPriority() const
{
    return priority;
}

bool WindowBlindTask::setBlindsPosition(int windowId, BlindsPosition position)
{
    for (auto& controller : controllers)
    {
        if (controller -> getWindowId() == windowId)
        {
            return controller -> setPosition(position);
        }
    }
    return false;
}

std::vector<std::pair<int, std::string>> WindowBlindTask::getStatusReport() const
{
    std::vector<std::pair<int, std::string>> report;
    
    for (const auto& controller : controllers)
    {
        std::stringstream ss;
        ss << "Window " << controller->getWindowId() << ": " 
           << controller->getPositionName() << " (" 
           << static_cast<int>(controller->getPosition()) << "%)";
        
        report.push_back({controller->getWindowId(), ss.str()});
    }
    
    return report;
}