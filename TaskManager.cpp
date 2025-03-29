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

bool TaskManager::hasReadyTasks() const
{
    return std::any_of(tasks.begin(), tasks.end(), [](const auto& task) 
    { return task -> isReady; });
}

std::unique_ptr<Task>& TaskManager::selectNextTask()
{
    std::lock_guard<std::mutex> lock(taskMutex);

    auto highestPriorityTask = std::max_element(tasks.begin(), tasks.end(),
    [](const auto&a, const auto& b)
    {
        if (!a -> isReady) return true;
        if (!b -> isReady) return false;
        return a -> getPriority() < b -> getPriority();
    });

    if (highestPriorityTask != tasks.end() && (*highestPriorityTask) -> isReady)
    {
        return *highestPriorityTask;
    }

    return tasks.front();
}

void TaskManager::executeTask(Task* task)
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastExecution = now - task -> lastExecutionTime;

    if (timeSinceLastExecution >= task -> timeSlice)
    {
        try
        {
            Logger::getInstance() -> log("Executing task: " + task -> getName());
            task -> execute();
            task -> lastExecutionTime = std::chrono::steady_clock::now();

            auto newTimeSlice = std::chrono::milliseconds(100 + (10 * task -> getPriority()));
            newTimeSlice = std::min(maxTimeSlice, std::max(minTimeSlice, newTimeSlice));
            task -> timeSlice = newTimeSlice;
        }
        catch(const std::exception& e)
        {
            Logger::getInstance() -> log("Error executing task: " + task -> getName() + " - " + e.what());
            task -> isReady = false;
        }
    }
}

void TaskManager::schedulerLoop()
{
     while (isRunning)
     {
        std::unique_lock<std::mutex> lock(taskMutex);

        if (tasks.empty())
        {
            scheduleCV.wait(lock, [this]()
            {
                return !isRunning || !tasks.empty();
            });
        }

        if (!hasReadyTasks())
        {
            scheduleCV.wait_for(lock, std::chrono::milliseconds(10));
            continue;
        }

        auto& nextTask = selectNextTask();
        lock.unlock();

        if (nextTask -> isReady)
        {
            executeTask(nextTask.get());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
     }
}


