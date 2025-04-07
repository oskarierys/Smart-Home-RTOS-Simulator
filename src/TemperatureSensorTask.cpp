#include "TemperatureSensorTask.hpp"
#include "Logger.hpp"
#include <sstream>
#include <ctime>

float TemperatureSensor::currentTemperature = 22.0f;  // Start with a reasonable default
std::mutex TemperatureSensor::temperatureMutex;

TemperatureSensor::TemperatureSensor(const std::string& sensorName)
    : Sensor(sensorName)
{
    std::random_device rd;
    rng = std::mt19937(rd());
    tempVariation = std::uniform_real_distribution<float>(-0.5f, 0.5f);
}

float TemperatureSensor::readValue()
{
    std::lock_guard<std::mutex> lock(valueMutex);
    return currentTemperature + tempVariation(rng);
}

float TemperatureSensor::getLastReading()
{
    std::lock_guard<std::mutex> lock(temperatureMutex);
    return currentTemperature;
}

void TemperatureSensor::setReading(float temperature)
{
    std::lock_guard<std::mutex> lock(temperatureMutex);
    currentTemperature = temperature;
}

TemperatureSensorTask::TemperatureSensorTask(const std::string& taskName, int taskPriority)
    : name(taskName), priority(taskPriority)
{
    sensor = std::make_unique<TemperatureSensor>("Main Temperature Sensor");
    lastExecuted = std::chrono::steady_clock::now();
    
    Logger::getInstance() -> log("Temperature sensor system initialized", true);
}

void TemperatureSensorTask::execute()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastExecuted).count();

    if (elapsed >= 30)
    {
        float simulatedTemp = simulateTemperature();

        TemperatureSensor::setReading(simulatedTemp);

        float reading = sensor->readValue();
        
        std::stringstream ss;
        ss << "Temperature updated: " << reading << "°C";
        Logger::getInstance() -> log(ss.str(), false);
        
        lastExecuted = now;
    }
}

float TemperatureSensorTask::simulateTemperature() const
{
    time_t now = time(0);
    struct tm timeInfo;
    
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    
    int hour = timeInfo.tm_hour;

    float baseTemp = 20.0f;
    if (hour >= 0 && hour < 6) 
    {
        baseTemp = 18.0f + (hour * 0.2f);
    } 
    else if (hour >= 6 && hour < 12) 
    {
        baseTemp = 19.2f + ((hour - 6) * 0.8f);
    } 
    else if (hour >= 12 && hour < 18) 
    {
        baseTemp = 24.0f + ((hour - 12) * 0.2f);
    } 
    else 
    {
        baseTemp = 25.2f - ((hour - 18) * 1.2f); 
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    
    return baseTemp + dis(gen);
}

const std::string& TemperatureSensorTask::getName() const
{
    return name;
}

int TemperatureSensorTask::getPriority() const
{
    return priority;
}