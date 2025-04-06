#include "WindowBlindController.hpp"
#include "Logger.hpp"
#include <sstream>
#include <vector>

static std::vector<WindowBlindController*> allBlinds;

WindowBlindController::WindowBlindController(const std::string& controllerName, int id)
    : Sensor(controllerName), 
      currentPosition(BlindsPosition::CLOSED),
      windowId(id),
      lastMoveTime(std::chrono::steady_clock::now())
{
    allBlinds.push_back(this);
}

float WindowBlindController::readValue()
{
    std::lock_guard<std::mutex> lock(valueMutex);
    return static_cast<float>(static_cast<int>(currentPosition));
}

bool WindowBlindController::setPosition(BlindsPosition position)
{
    std::lock_guard<std::mutex> lock(valueMutex);

    auto now = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastMoveTime).count();

    if (elapsedMs < MOVE_COOLDOWN_MS)
    {
        return false;
    }

    lastMoveTime = now;

    if (position != currentPosition)
    {
        std::stringstream logMsg;
        logMsg << "Window " << windowId << " blinds moving from " 
               << static_cast<int>(currentPosition) << "% to " 
               << static_cast<int>(position) << "%";
               
        Logger::getInstance() -> log(logMsg.str(), true);
        
        currentPosition = position;
    }

    return true;
}

BlindsPosition WindowBlindController::getPosition() const
{
    return currentPosition;
}

int WindowBlindController::getWindowId() const
{
    return windowId;
}

std::string WindowBlindController::getPositionName() const
{
    switch (currentPosition)
    {
        case BlindsPosition::CLOSED: 
            return "CLOSED";
        case BlindsPosition::QUARTER_OPEN: 
            return "QUARTER_OPEN";
        case BlindsPosition::HALF_OPEN: 
            return "HALF_OPEN";
        case BlindsPosition::THREE_QUARTERS_OPEN: 
            return "THREE_QUARTERS_OPEN";
        case BlindsPosition::OPEN: 
            return "OPEN";
        default: 
            return "UNKNOWN";
    }
}

void WindowBlindController::openAllBlinds()
{
    for (auto& blind : allBlinds)
    {
        blind -> setPosition(BlindsPosition::OPEN);
    }

    Logger::getInstance() -> log("Command: Opening all blinds", true);
}

void WindowBlindController::closeAllBlinds()
{
    for (auto& blind : allBlinds)
    {
        blind -> setPosition(BlindsPosition::CLOSED);
    }

    Logger::getInstance() -> log("Command: Closing all blinds", true);
}