#pragma once
#include "messagequeue/message.hpp"
#include <memory>

namespace CoffeeShop
{
class MessageBus
{
public:
    MessageBus();
    ~MessageBus();
    void push(const Message& message);
    Message get();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl {};
};


}
