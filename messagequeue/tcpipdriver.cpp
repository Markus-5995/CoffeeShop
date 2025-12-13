#include "tcpipdriver.hpp"
#include <iostream>
namespace CoffeeShop
{

TcpIpDriver::TcpIpDriver()
{

}

void TcpIpDriver::registerAsServer(MessageBus::ProducerId)
{
    communciation.emplace<TcpServer>();
}

void TcpIpDriver::registerAsClient(MessageBus::ConsumerId)
{
    communciation.emplace<TcpClient>();
}

void TcpIpDriver::push(const Message &message)
{
    if (auto* server = std::get_if<TcpServer>(&communciation))
    {
        return server->push(message);
    }
    throw std::runtime_error("Cannot push messages as client!");

}

std::optional<Message> TcpIpDriver::tryPop(MessageBus::ConsumerId)
{
    if (auto* client = std::get_if<TcpClient>(&communciation))
    {
        return client->tryPop();
    }
    throw std::runtime_error("Cannot get messages as server!");
}

std::optional<Message> TcpIpDriver::waitAndTryPop(MessageBus::ConsumerId)
{
    if (auto* client = std::get_if<TcpClient>(&communciation))
    {
        return client->waitAndTryPop();
    }
    throw std::runtime_error("Cannot get messages as server!");
}

bool TcpIpDriver::alive() const
{
    return std::visit(
        [](auto& tcp) {
            using T = std::decay_t<decltype(tcp)>;

            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return false;
            }
            else
            {
                return tcp.alive();
            }
        },
        communciation);
}



} // namespace CoffeeShop
