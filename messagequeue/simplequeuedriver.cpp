#include "simplequeuedriver.hpp"

namespace CoffeeShop
{
    void SimpleQueueDriver::push(const Message& message)
    {
        std::unique_lock lock(mtx);
        queue.push(message);
        condition.notify_one();
    }
    std::optional<Message> SimpleQueueDriver::tryPop(MessageBus::ConsumerId)
    {
        std::unique_lock lock(mtx);
        return popInternal();
    }
    std::optional<Message> SimpleQueueDriver::waitAndTryPop(MessageBus::ConsumerId)
    {
        std::unique_lock lock(mtx);
        condition.wait(lock, [this]()
            {
                return !queue.empty() || !isAlive;
            });
        return popInternal();
    }
    void SimpleQueueDriver::close()
    {
        std::unique_lock lock(mtx);
        isAlive = false;
        condition.notify_all();
    }
    std::optional<Message> SimpleQueueDriver::popInternal()
    {
        if (queue.empty())
        {
            return {};
        }
        auto m = queue.front();
        queue.pop();
        return m;
    }
} // namespace CoffeeShop
