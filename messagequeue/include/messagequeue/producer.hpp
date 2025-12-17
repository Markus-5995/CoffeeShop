#pragma once
#include "messagebus.hpp"
#include <memory>
namespace CoffeeShop
{
class MessageBus;

class MSGQ_API Producer
{
public:
    Producer(const std::shared_ptr<MessageBus>& bus);
    void push(const Message& message);
    ~Producer();

private:
    std::shared_ptr<MessageBus> m_bus;
    MessageBus::ProducerId m_id {};
};

}
