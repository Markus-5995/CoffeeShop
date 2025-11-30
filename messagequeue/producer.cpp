#include "messagequeue/producer.hpp"
#include "messagebus.hpp"
namespace CoffeeShop
{

Producer::~Producer() = default;

Producer::Producer() :
    m_bus(std::make_unique<MessageBus>())
{

}

void Producer::push(const Message &message)
{
    m_bus->place(message);
}


}
