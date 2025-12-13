#include "messagequeue/consumer.hpp"
namespace CoffeeShop
{
Consumer::Consumer(const std::shared_ptr<MessageBus> &bus) :
    m_bus(bus)
{
    m_id = m_bus->registerConsumer();
}

Consumer::~Consumer() = default;

std::optional<Message> Consumer::get()
{
    if (m_bus != nullptr)
    {
        return m_bus->get(m_id);
    }
    return {};
}

std::optional<Message> Consumer::waitForNext()
{
    if (m_bus != nullptr)
    {
        return m_bus->waitForNext(m_id);
    }
    return {};
}

bool Consumer::alive() const
{
    if (m_bus != nullptr)
    {
        return m_bus->alive();
    }
    return false;
}


}


