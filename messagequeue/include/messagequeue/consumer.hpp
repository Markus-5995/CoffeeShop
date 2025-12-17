#pragma once
#include "message.hpp"
#include <memory>
#include <optional>
#include "messagequeue/messagebus.hpp"
namespace CoffeeShop
{
class MessageBus;

class MSGQ_API Consumer
{
public:
    Consumer(const std::shared_ptr<MessageBus>& bus);
    std::optional<Message> get();
    std::optional<Message> waitForNext();
    bool alive() const;
    ~Consumer();

private:
    std::shared_ptr<MessageBus> m_bus;
    MessageBus::ConsumerId m_id;
};

}
