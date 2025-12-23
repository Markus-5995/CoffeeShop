#pragma once
#include <string>
#include <vector>
#include "messagequeueexport.hpp"
namespace CoffeeShop
{
class MSGQ_API Message
{
public:
    using PayLoad = std::vector<std::uint8_t>;

    Message() = default;
    explicit Message(std::string_view message) : m_payload(message.begin(), message.end()) {}
    explicit Message(PayLoad&& payload) : m_payload(std::move(payload)) {}
    
    operator std::string()
    {
        return std::string(m_payload.begin(), m_payload.end());
    }

    size_t size() const
    {
        return m_payload.size();
    }


    const PayLoad& payload() const
    {
        return m_payload;
    }
private:
    PayLoad m_payload{};
};

}
