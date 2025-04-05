#pragma once

#include "TaskManager.hpp"
#include "WindowBlindController.hpp"
#include <memory>
#include <string>
#include <chrono>

class WindowBlindTask : public Task
{
private:
    std::vector<std::unique_ptr<WindowBlindController>> controllers;
    std::string name;
    int priority;
    std::chrono::steady_clock::time_point lastExecuted;
    float simulateOutdoorLight() const;
    void applyTimeBasedRules();
    void applyLightBasedRules(float lightLevel);

public:
    WindowBlindTask(const std::string& taskName, int taskPriority);
    void execute() override;
    const std::string& getName() const override;
    int getPriority() const override;
    bool setBlindsPosition(int windowId, BlindsPosition position);
    std::vector<std::pair<int, std::string>> getStatusReport() const;
};