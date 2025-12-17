#pragma once
#include "messagequeue/message.hpp"
#include <memory>
#include <optional>
#include "messagequeueexport.hpp"

namespace CoffeeShop
{

class MSGQ_API MessageBus
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
    std::optional<Message> get(ConsumerId id);
    std::optional<Message> waitForNext(ConsumerId id);
    bool alive() const;

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl {};
};


}
