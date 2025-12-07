#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"
namespace CoffeeShop
{
Consumer::Consumer(const std::shared_ptr<MessageBus> &bus) :
    m_bus(bus)
{

}

Consumer::~Consumer() = default;

std::optional<Message> Consumer::get()
{
    if (m_bus != nullptr)
    {
        return m_bus->get();
    }
    return {};
}


}


