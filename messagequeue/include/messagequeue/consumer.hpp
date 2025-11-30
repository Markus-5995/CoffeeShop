#pragma once
#include "message.hpp"
#include <memory>
namespace CoffeeShop
{
class MessageBus;

class Consumer
{
public:
    Consumer();
    Message get();
    ~Consumer();

private:
    std::unique_ptr<MessageBus> m_bus;
};

}
