#include "messagequeue/messagebus.hpp"
#include "driverimplementations.hpp"
#include <mutex>
namespace CoffeeShop
{

struct MessageBus::Impl
{
    std::mutex mtx {};
    std::variant<SimpleQueueImpl, MultiConsumerQueueImpl, TcpIpDriver> m_driver;
    MessageBus::ConsumerId nextConsumerId {0};
    MessageBus::ProducerId nextProducerId {0};
};

MessageBus::MessageBus(Driver driver) :
    m_impl(new Impl())
{
    switch(driver)
    {
    case SimpleQueue:
        m_impl->m_driver.emplace<SimpleQueueImpl>();
        break;
    case MultiConsumerQueue:
        m_impl->m_driver.emplace<MultiConsumerQueueImpl>();
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
    std::unique_lock lock(m_impl->mtx);

    uint32_t& nextId = m_impl->nextProducerId;
    std::visit(DriverRegisterProducerImpl{++nextId}, m_impl->m_driver);
    return nextId;
}

uint32_t MessageBus::registerConsumer()
{
    std::unique_lock lock(m_impl->mtx);

    uint32_t& nextId = m_impl->nextConsumerId;
    std::visit(DriverRegisterConsumerImpl{++nextId}, m_impl->m_driver);
    return nextId;
}

void MessageBus::push(ProducerId, const Message &message)
{
    std::unique_lock lock(m_impl->mtx);

    std::visit(DriverPushImpl{message}, m_impl->m_driver);
}

Message MessageBus::get(ConsumerId id)
{
    std::unique_lock lock(m_impl->mtx);

    return std::visit(DriverGetImpl{id}, m_impl->m_driver);


}

}
