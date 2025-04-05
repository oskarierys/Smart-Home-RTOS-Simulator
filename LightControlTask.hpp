#pragma once

#include "TaskManager.hpp"
#include "LightController.hpp"
#include <memory>
#include <string>
#include <chrono>
#include <vector>

class LightControlTask : public Task {
private:
    std::vector<std::unique_ptr<LightController>> controllers;
    std::string name;
    int priority;
    std::chrono::steady_clock::time_point lastExecuted;

    float simulateMotion(int roomId) const;

    void applyTimeBasedRules();
    void applyMotionBasedRules();

public:
    LightControlTask(const std::string& taskName, int taskPriority);
    void execute() override;
    const std::string& getName() const override;
    int getPriority() const override;

    bool setLight(int roomId, bool on);
    bool setBrightness(int roomId, LightBrightness level);

    std::vector<std::pair<int, std::string>> getStatusReport() const;
};