#pragma once
#include "messagequeue/message.hpp"
#include <memory>

namespace CoffeeShop
{


class MessageBus
{
public:
    using ConsumerId = uint32_t;
    using ProducerId = uint32_t;
    enum Driver
    {
        Undefined,
        SimpleQueue,
        MultiConsumerQueue,
        TcpIp
    };
    MessageBus(Driver driver);
    ~MessageBus();
    ConsumerId registerConsumer();
    ProducerId registerProducer();
    void push(ProducerId id, const Message& message);
    Message get(ConsumerId id);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl {};
};


}
