#pragma once
#include <queue>
#include "messagequeue/messagebus.hpp"
#include <mutex>
#include <condition_variable>
namespace CoffeeShop
{

class SimpleQueueDriver
{
public:

    void registerAsServer(MessageBus::ProducerId) {}
    void registerAsClient(MessageBus::ConsumerId) {}
    bool alive() const {return isAlive;}

    void push(const Message& message)
    {
        std::unique_lock lock(mtx);
        queue.push(message);
        condition.notify_one();
    }

    std::optional<Message> tryPop(MessageBus::ConsumerId)
    {
        std::unique_lock lock(mtx);
        return popInternal();
    }

    std::optional<Message> waitAndTryPop(MessageBus::ConsumerId)
    {
        std::unique_lock lock(mtx);
        condition.wait(lock, [this]()
                       {
                           return ! queue.empty() || ! isAlive;
                       });
        return popInternal();
    }

    void close()
    {
        std::unique_lock lock(mtx);
        isAlive = false;
        condition.notify_all();
    }
private:
    std::optional<Message> popInternal()
    {
        if (queue.empty())
        {
            return {};
        }
        auto m = queue.front();
        queue.pop();
        return m;
    }
    std::mutex mtx;
    std::condition_variable condition {};
    std::queue<Message> queue {};
    bool isAlive {true};
};

} // namespace CoffeeShop
