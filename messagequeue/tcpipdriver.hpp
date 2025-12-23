#pragma once
#include "tcpserver.hpp"
#include "tcpclient.hpp"
#include "messagequeue/messagebus.hpp"
#include <variant>
namespace CoffeeShop
{
class TcpIpDriver
{
public:
    TcpIpDriver();
    void registerAsServer(MessageBus::ProducerId);
    void registerAsClient(MessageBus::ConsumerId);

    void push(const Message& message);
    std::optional<Message> tryPop(MessageBus::ConsumerId id);
    std::optional<Message> waitAndTryPop(MessageBus::ConsumerId id);
    bool alive() const;
private:
    std::variant<std::monostate, TcpServer, TcpClient> communciation;
};

} // namespace CoffeeShop
