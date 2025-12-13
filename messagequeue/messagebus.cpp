#include "messagequeue/messagebus.hpp"
#include "driverconcept.hpp"
#include <iostream>
namespace CoffeeShop
{

struct MessageBus::Impl
{
    std::variant<SimpleQueueDriver, MultiConsumerQueueDriver, TcpIpDriver> m_driver;
    MessageBus::ConsumerId nextConsumerId {0};
    MessageBus::ProducerId nextProducerId {0};
};

MessageBus::MessageBus(Driver driver) :
    m_impl(new Impl())
{
    switch(driver)
    {
    case SimpleQueue:
        m_impl->m_driver.emplace<SimpleQueueDriver>();
        break;
    case MultiConsumerQueue:
        m_impl->m_driver.emplace<MultiConsumerQueueDriver>();
        break;
    case TcpIp:
        m_impl->m_driver.emplace<TcpIpDriver>();
        break;
    default:
        throw std::runtime_error("No valid driver selected!");
    }
}

MessageBus::~MessageBus() = default;

MessageBus::ProducerId MessageBus::registerProducer()
{
    uint32_t& nextId = m_impl->nextProducerId;
    nextId++;
    std::visit([nextId](auto& driver) { driver.registerAsServer(nextId);},
               m_impl->m_driver);
    return nextId;
}

uint32_t MessageBus::registerConsumer()
{
    uint32_t& nextId = m_impl->nextConsumerId;
    nextId++;
    std::visit([nextId](auto& driver) { driver.registerAsClient(nextId); },
               m_impl->m_driver);
    return nextId;
}

void MessageBus::push(ProducerId, const Message &message)
{
    std::visit([message](auto& driver) { driver.push(message); },
               m_impl->m_driver);
}

std::optional<Message> MessageBus::get(ConsumerId id)
{
    return std::visit([id](auto& driver) { return driver.tryPop(id);},
                      m_impl->m_driver);
}

std::optional<Message> MessageBus::waitForNext(ConsumerId id)
{
    auto m = std::visit([id](auto& driver) { return driver.waitAndTryPop(id);},
                        m_impl->m_driver);

    static int i {};
    std::cout << "Waited for message #" << ++i << std::endl;
    return m;
}

bool MessageBus::alive() const
{
    return std::visit([](auto& driver) {return driver.alive();},
                      m_impl->m_driver);
}

}
