#include "multiconsumerqueuedriver.hpp"

namespace CoffeeShop {
    void MultiConsumerQueueDriver::registerAsClient(MessageBus::ConsumerId id)
    {
        std::lock_guard lock(mtx);
        if (!multiQueue.contains(id))
        {
            multiQueue[id] = std::make_shared<SimpleQueueDriver>();
        }
    }
    bool MultiConsumerQueueDriver::alive() const
    {
        return std::ranges::any_of(multiQueue,
            [](auto& pair)
            {
                return pair.second->alive();
            });
    }
    void MultiConsumerQueueDriver::push(const Message& message)
    {
        std::lock_guard lock(mtx);
        std::for_each(multiQueue.begin(), multiQueue.end(), [message](auto& pair)
            {
                pair.second->push(message);
            });
    }
    std::optional<Message> MultiConsumerQueueDriver::tryPop(MessageBus::ConsumerId id)
    {
        if (auto queue = getQueue(id))
        {
            return queue->tryPop(id);
        }
        return {};
    }
    std::optional<Message> MultiConsumerQueueDriver::waitAndTryPop(MessageBus::ConsumerId id)
    {
        if (auto queue = getQueue(id))
        {
            return queue->waitAndTryPop(id);
        }
        return {};
    }
    void MultiConsumerQueueDriver::close()
    {
        std::lock_guard lock(mtx);
        std::for_each(multiQueue.begin(), multiQueue.end(), [](auto& pair)
            {
                pair.second->close();
            });
    }
    std::shared_ptr<SimpleQueueDriver> MultiConsumerQueueDriver::getQueue(MessageBus::ConsumerId id)
    {
        std::lock_guard lock(mtx);
        if (multiQueue.contains(id))
        {
            return multiQueue[id];
        }
        return {};
    }
} // namespace CoffeeShop
