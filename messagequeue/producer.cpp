#include "messagequeue/producer.hpp"
#include "messagequeue/messagebus.hpp"
#include <memory>
namespace CoffeeShop
{

Producer::Producer(const std::shared_ptr<MessageBus> &bus) :
    m_bus(bus)
{
    if (m_bus != nullptr)
    {
        m_id = m_bus->registerProducer();
    }
}

Producer::~Producer() = default;

void Producer::push(const Message &message)
{
    if (m_bus != nullptr)
    {
        m_bus->push(m_id, message);
    }
}


}
