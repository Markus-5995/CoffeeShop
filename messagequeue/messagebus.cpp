#include "messagequeue/messagebus.hpp"
#include <queue>
#include <mutex>
#include <variant>
#include <unordered_map>
#include <algorithm>
namespace CoffeeShop
{
using SimpleQueueImpl = std::queue<Message>;
using MultiConsumerQueueImpl = std::unordered_map<MessageBus::ConsumerId, std::queue<Message>>;


struct DriverRegisterImpl
{
    MessageBus::ConsumerId id{};
    void operator()(SimpleQueueImpl& queue)
    {
        if (id> 1)
        {
            throw std::runtime_error("Simple Queue only supports single Consumer!");
        }
    }
    void operator()(MultiConsumerQueueImpl& multiQueue)
    {
        multiQueue[id] = SimpleQueueImpl();
    }
};

struct DriverPushImpl
{
    const Message& message;
    void operator()(SimpleQueueImpl& queue)
    {
        queue.push(message);
    }
    void operator()(MultiConsumerQueueImpl& multiQueue)
    {
        std::for_each(multiQueue.begin(), multiQueue.end(), [this](auto& pair)
                      {
                          DriverPushImpl{message}(pair.second);
                      });
    }
};

struct DriverGetImpl
{
    MessageBus::ConsumerId id{};
    Message operator()(SimpleQueueImpl& queue)
    {
        if (queue.empty())
        {
            return Message("");
        }
        Message m = queue.front();
        queue.pop();
        return m;
    }
    Message operator()(MultiConsumerQueueImpl& multiQueue)
    {
        if (! multiQueue.contains(id))
        {
            return Message("");
        }
        return DriverGetImpl{id}(multiQueue[id]);
    }
};

struct MessageBus::Impl
{
    std::mutex mtx {};
    std::variant<SimpleQueueImpl, MultiConsumerQueueImpl> m_driver;
    uint32_t nextId {0};
};

MessageBus::MessageBus(Driver driver) :
    m_impl(new Impl())
{
    switch(driver)
    {
    case SimpleQueue:
        m_impl->m_driver = SimpleQueueImpl();
        break;
    case MultiConsumerQueue:
        m_impl->m_driver = MultiConsumerQueueImpl();
        break;
    default:
        throw std::runtime_error("No valid driver selected!");
    }
}

MessageBus::~MessageBus() = default;

uint32_t MessageBus::registerConsumer()
{
    std::unique_lock lock(m_impl->mtx);

    uint32_t& nextId = m_impl->nextId;
    std::visit(DriverRegisterImpl{++nextId}, m_impl->m_driver);
    return nextId;
}

void MessageBus::push(const Message &message)
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
