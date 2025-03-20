#include "TaskManager.hpp"
#include "Logger.hpp"

TaskManager* TaskManager::instance = nullptr;

TaskManager* TaskManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new TaskManager();
    }

    return instance;
}

void TaskManager::addTask(std::unique_ptr<Task> task)
{
    std::lock_guard<std::mutex> lock(taskMutex);
    tasks.push_back(std::move(task));
}

void TaskManager::startScheduler()
{
    if (!isRunning)
    {
        isRunning = true;
        schedulerThread = std::thread(&TaskManager::schedulerLoop, this);
        Logger::getInstance() -> log("Scheduler started");
    }
}

void TaskManager::stopScheduler()
{
    if (isRunning)
    {
        isRunning = false;
        scheduleCV.notify_all();
        if (schedulerThread.joinable())
        {
            schedulerThread.join();
        }

        Logger::getInstance() -> log("Scheduler stopped");
    }
}

TaskManager::~TaskManager()
{
    stopScheduler();
    delete instance;
}



