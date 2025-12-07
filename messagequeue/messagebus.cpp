#include "messagequeue/messagebus.hpp"
#include <queue>
#include <mutex>
#include <variant>
namespace CoffeeShop
{

using SimpleQueue = std::queue<Message>;


struct MessageBus::Impl
{
    std::mutex mtx {};
    std::variant<SimpleQueue> m_driver {};
};

MessageBus::MessageBus() :
    m_impl(new Impl())
{
    m_impl->m_driver = SimpleQueue();
}

MessageBus::~MessageBus() = default;

void MessageBus::push(const Message &message)
{
    std::unique_lock lock(m_impl->mtx);

    std::visit([message](auto&& bus) { bus.push(message); }, m_impl->m_driver);
}

Message MessageBus::get()
{
    std::unique_lock lock(m_impl->mtx);

    return std::visit([](auto&& bus)
               {
                   if (bus.empty())
                   {
                       return Message(std::string());
                   }
                   auto messsage = bus.front();
                   bus.pop();
                   return messsage;
               }, m_impl->m_driver);


}

}
