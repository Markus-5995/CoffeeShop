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
    void push(const Message& message);
    std::optional<Message> tryPop(MessageBus::ConsumerId);
    std::optional<Message> waitAndTryPop(MessageBus::ConsumerId);
    void close();
private:
    std::optional<Message> popInternal();
    std::mutex mtx;
    std::condition_variable condition {};
    std::queue<Message> queue {};
    bool isAlive {true};
};

} // namespace CoffeeShop
