#pragma once
#include "simplequeuedriver.hpp"
#include <algorithm>
#include <unordered_map>
namespace CoffeeShop
{

class MultiConsumerQueueDriver
{
public:

    void registerAsServer(MessageBus::ProducerId) {}
    void registerAsClient(MessageBus::ConsumerId id)
    {
        std::lock_guard lock(mtx);
        if (! multiQueue.contains(id))
        {
            multiQueue[id] = std::make_shared<SimpleQueueDriver>();
        }
    }

    bool alive() const
    {
        return std::ranges::any_of(multiQueue,
                                   [] (auto& pair)
                                   {
                                       return pair.second->alive();
                                   });
    }

    void push(const Message& message)
    {
        std::lock_guard lock(mtx);
        std::for_each(multiQueue.begin(), multiQueue.end(), [message](auto& pair)
                      {
                          pair.second->push(message);
                      });
    }

    std::optional<Message> tryPop(MessageBus::ConsumerId id)
    {
        if (auto queue = getQueue(id))
        {
            return queue->tryPop(id);
        }
        return {};
    }

    std::optional<Message> waitAndTryPop(MessageBus::ConsumerId id)
    {
        if (auto queue = getQueue(id))
        {
            return queue->waitAndTryPop(id);
        }
        return {};
    }

    void close()
    {
        std::lock_guard lock(mtx);
        std::for_each(multiQueue.begin(), multiQueue.end(), [](auto& pair)
                      {
                          pair.second->close();
                      });
    }

private:
    std::shared_ptr<SimpleQueueDriver> getQueue(MessageBus::ConsumerId id)
    {
        std::lock_guard lock(mtx);
        if (multiQueue.contains(id))
        {
            return multiQueue[id];
        }
        return {};
    }

    std::mutex mtx {};
    std::unordered_map<MessageBus::ConsumerId, std::shared_ptr<SimpleQueueDriver>> multiQueue {};
};

} // namespace CoffeeShop
