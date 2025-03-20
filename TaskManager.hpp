#pragma once

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>
#include <algorithm>

class Task {
public:
    virtual ~Task() = default;
    virtual void execute() = 0;
    virtual const std::string& getName() const = 0;
    virtual int getPriority() const = 0;

    std::chrono::microseconds timeSlice{100};
    std::chrono::steady_clock::time_point lastExecutionTime;
    bool isReady{true};
};

class TaskManager {
private:
    std::vector<std::unique_ptr<Task>> tasks;
    std::mutex taskMutex;
    std::condition_variable scheduleCV;
    std::atomic<bool> isRunning{false};
    std::thread schedulerThread;

    const std::chrono::milliseconds minTimeSlice{50};
    const std::chrono::milliseconds maxTimeSlice{200};
    
    static TaskManager* instance;
    TaskManager() = default;

    void schedulerLoop();
    std::unique_ptr<Task>& selectNextTask();
    void executeTask(Task* task);
    bool hasReadyTasks() const;

public:
    static TaskManager* getInstance();
    void addTask(std::unique_ptr<Task> task);
    void startScheduler();
    void stopScheduler();
    ~TaskManager();
}; 