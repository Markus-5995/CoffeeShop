#pragma once
#include <string>
#include <vector>
#include "messagequeueexport.hpp"
namespace CoffeeShop
{
struct MSGQ_API Message
{
    Message(std::string_view message) : payload(message.begin(), message.end()) {}
    std::vector<std::uint8_t> payload {};
    operator std::string()
    {
        return std::string(payload.begin(), payload.end());
    }
};

}
