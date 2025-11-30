#pragma once
#include "message.hpp"
#include <memory>
namespace CoffeeShop
{
class MessageBus;

class Producer
{
public:
    Producer();
    void push(const Message& message);
    ~Producer();

private:
    std::unique_ptr<MessageBus> m_bus;
};

}
