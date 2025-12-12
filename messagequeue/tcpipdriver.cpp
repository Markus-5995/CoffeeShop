#include "tcpipdriver.hpp"
#include <iostream>
namespace CoffeeShop
{

TcpIpDriver::TcpIpDriver()
{

}

void TcpIpDriver::registerAsServer()
{
    communciation.emplace<TcpServer>();
}

void TcpIpDriver::registerAsClient()
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

Message TcpIpDriver::get()
{
    if (auto* client = std::get_if<TcpClient>(&communciation))
    {
        return client->get();
    }
    throw std::runtime_error("Cannot get messages as server!");
}



} // namespace CoffeeShop
