#pragma once
#include <string>
#include <vector>
namespace CoffeeShop
{
struct Message
{
    Message(std::string_view message) : payload(message.begin(), message.end()) {}
    std::vector<u_int8_t> payload {};
    operator std::string()
    {
        return std::string(payload.begin(), payload.end());
    }
};

}
