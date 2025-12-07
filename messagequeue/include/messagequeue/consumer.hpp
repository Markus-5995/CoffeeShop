#pragma once
#include "message.hpp"
#include <memory>
#include <optional>
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
};

}
