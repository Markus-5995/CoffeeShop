#pragma once
#include "message.hpp"
#include <memory>
#include <optional>
#include "messagequeue/messagebus.hpp"
namespace CoffeeShop
{
class MessageBus;

class Consumer
{
public:
    Consumer(const std::shared_ptr<MessageBus>& bus);
    std::optional<Message> get();
    ~Consumer();

private:
    std::shared_ptr<MessageBus> m_bus;
    MessageBus::ConsumerId m_id;
};

}
