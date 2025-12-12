#pragma once
#include "tcpimpl.hpp"
namespace CoffeeShop
{
class TcpIpDriver
{
public:
    TcpIpDriver();
    void registerAsServer();
    void registerAsClient();

    void push(const Message& message);
    Message get();
private:
    std::variant<std::monostate, TcpServer, TcpClient> communciation;
};

} // namespace CoffeeShop
