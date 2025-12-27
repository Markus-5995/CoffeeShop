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
    void registerAsClient(MessageBus::ConsumerId id);
    bool alive() const;
    void push(const Message& message);
    std::optional<Message> tryPop(MessageBus::ConsumerId id);
    std::optional<Message> waitAndTryPop(MessageBus::ConsumerId id);
    void close();

private:
    std::shared_ptr<SimpleQueueDriver> getQueue(MessageBus::ConsumerId id);

    std::mutex mtx {};
    std::unordered_map<MessageBus::ConsumerId, std::shared_ptr<SimpleQueueDriver>> multiQueue {};
};

} // namespace CoffeeShop
