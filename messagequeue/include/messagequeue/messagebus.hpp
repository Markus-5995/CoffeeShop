#pragma once
#include "messagequeue/message.hpp"
#include <memory>

namespace CoffeeShop
{


class MessageBus
{
public:
    using ConsumerId = uint32_t;
    enum Driver
    {
        Undefined,
        SimpleQueue,
        MultiConsumerQueue
    };
    MessageBus(Driver driver);
    ~MessageBus();
    ConsumerId registerConsumer();
    void push(const Message& message);
    Message get(ConsumerId id);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl {};
};


}
