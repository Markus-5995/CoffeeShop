#pragma once
#include <queue>
#include "messagequeue/message.hpp"
#include <mutex>

namespace CoffeeShop
{
class MessageBus
{
public:
    void place(const Message& message)
    {
        std::unique_lock lock(mtx);

        m_driver.push(message);
    }
    Message get()
    {
        std::unique_lock lock(mtx);

        if (m_driver.empty())
        {
            return Message(std::string());
        }
        auto messsage = m_driver.front();
        m_driver.pop();
        return messsage;
    }

private:
    std::mutex mtx {};
    static inline std::queue<Message> m_driver{};
};


}
