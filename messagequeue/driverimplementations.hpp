#pragma once
#include "messagequeue/messagebus.hpp"
#include "tcpipdriver.hpp"
#include <queue>
#include <variant>
#include <unordered_map>
#include <algorithm>
namespace CoffeeShop
{
using SimpleQueueImpl = std::queue<Message>;
using MultiConsumerQueueImpl = std::unordered_map<MessageBus::ConsumerId, std::queue<Message>>;

struct DriverRegisterProducerImpl
{
    MessageBus::ConsumerId id{};
    void operator()(SimpleQueueImpl&)
    {
        onlyOneProducerAllowed();
    }
    void operator()(MultiConsumerQueueImpl&)
    {
        onlyOneProducerAllowed();
    }
    void operator()(TcpIpDriver& driver)
    {
        driver.registerAsServer();
    }

    void onlyOneProducerAllowed()
    {
        if (id> 1)
        {
            throw std::runtime_error("Driver only supports single Producer!");
        }
    }
};

struct DriverRegisterConsumerImpl
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

    void operator()(TcpIpDriver& driver)
    {
        driver.registerAsClient();
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
    void operator()(TcpIpDriver& driver)
    {
        driver.push(message);
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
    Message operator()(TcpIpDriver& driver)
    {
        return driver.get();
    }
};

}
