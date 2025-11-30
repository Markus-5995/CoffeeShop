#include "messagequeue/consumer.hpp"
#include "messagebus.hpp"
namespace CoffeeShop
{


Consumer::Consumer() :
    m_bus(std::make_unique<MessageBus>())
{

}


Consumer::~Consumer() = default;


Message Consumer::get()
{
    return m_bus->get();
}

}
