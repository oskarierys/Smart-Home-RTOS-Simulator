#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <mutex>

class EventManager {
private:
    static EventManager* instance;
    std::map<std::string, std::vector<std::function<void(void*)>>> subscribers;
    std::mutex eventMutex;

    EventManager() = default;

public:
    static EventManager* getInstance();
    void subscribe(const std::string& eventType, std::function<void(void*)> callback);
    void publish(const std::string& eventType, void* data);
    
    ~EventManager();
};