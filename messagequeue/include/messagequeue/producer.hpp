#pragma once
#include "message.hpp"
#include <memory>
namespace CoffeeShop
{
class MessageBus;

class Producer
{
public:
    Producer(const std::shared_ptr<MessageBus>& bus);
    void push(const Message& message);
    ~Producer();

private:
    std::shared_ptr<MessageBus> m_bus;
};

}
