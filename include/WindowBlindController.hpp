#pragma once

#include <string>
#include <mutex>
#include <chrono>
#include "Sensor.hpp"

enum class BlindsPosition
{
    CLOSED = 0,
    QUARTER_OPEN = 25,
    HALF_OPEN = 50,
    THREE_QUARTERS_OPEN = 75,
    OPEN = 100
};

class WindowBlindController : Sensor
{
private:
    BlindsPosition currentPosition;
    int windowId;
    std::chrono::steady_clock::time_point lastMoveTime;
    static const int MOVE_COOLDOWN_MS = 5000;

public:
    explicit WindowBlindController(const std::string& controllerName, int id);
    float readValue() override;
    bool setPosition(BlindsPosition position);
    BlindsPosition getPosition() const;
    int getWindowId() const;
    std::string getPositionName() const;

    static void openAllBlinds();
    static void closeAllBlinds();
};
