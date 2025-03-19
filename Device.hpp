#pragma once 

#include <string>
#include <mutex>

class Device {
protected:
    std::string name;
    bool state;
    std::mutex stateMutex;  

public:
    explicit Device(const std::string& deviceName);
    virtual ~Device() = default;
    virtual void setState(bool newState) = 0;
    virtual bool getState() const = 0;
    const std::string& getName() const;
};

class Light : public Device {
public:
    explicit Light(const std::string& name);
    void setState(bool newState) override;
    bool getState() const override;    
};