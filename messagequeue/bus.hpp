#pragma once
#include <queue>
#include "messagequeue/message.hpp"

namespace CoffeeShop
{
class MessageBus
{
public:
    void place(const Message& message)
    {
        m_driver.push(message);
    }
    Message get()
    {
        auto messsage = m_driver.front();
        m_driver.pop();
        return messsage;
    }
private:
    static inline std::queue<Message> m_driver{};
};


}
