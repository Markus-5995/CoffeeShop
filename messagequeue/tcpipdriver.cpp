#include "tcpipdriver.hpp"

namespace CoffeeShop
{

TcpIpDriver::TcpIpDriver() :
    io(std::make_shared<boost::asio::io_context>()),
    workGuard(boost::asio::make_work_guard(*io)),
    thread([this](){io->run();})
{

}

TcpIpDriver::~TcpIpDriver()
{
    workGuard.reset();
    io->stop();
}

void TcpIpDriver::registerAsServer()
{
    communciation.emplace<TcpServer>(io);
}

void TcpIpDriver::registerAsClient()
{
    communciation.emplace<TcpClient>(io);
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

Resources::Resources(std::shared_ptr<boost::asio::io_context> context) :
    ioContext(context),
    socket(*ioContext)
{

}

TcpServer::TcpServer(std::shared_ptr<boost::asio::io_context> context) :
    Resources(context),
    acceptor(*ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), TcpIpDriver::port))

{
    acceptor.async_accept(socket, std::bind(&TcpServer::writeCompleted,
                                            this, boost::asio::placeholders::error));
}

void TcpServer::push(const Message &message)
{
    std::unique_lock lock(mtx);
    messageQueue.push(message);
}

void TcpServer::writeCompleted(const boost::system::error_code &error)
{
    if (!error)
    {
        std::unique_lock lock(mtx);
        if (! messageQueue.empty())
        {
            std::string m = messageQueue.front();
            messageQueue.pop();
            if (m.size() > buffer.size())
            {
                throw std::runtime_error("We are in trouble!");
            }
            std::fill(buffer.begin(), buffer.end(), 0);
            std::copy(m.begin(), m.end(), buffer.begin());
            socket.async_write_some(boost::asio::buffer(buffer.data(), buffer.size()),
                                    std::bind(&TcpServer::writeCompleted,
                                              this, boost::asio::placeholders::error));
        }
    }
}

TcpClient::TcpClient(std::shared_ptr<boost::asio::io_context> context) :
    Resources(context)
{
    auto addr = boost::asio::ip::make_address(TcpIpDriver::hostAddress);

    socket.async_connect(boost::asio::ip::tcp::endpoint(addr, TcpIpDriver::port),
                         [this](const boost::system::error_code &error)
                         {
                             if (! error)
                             {
                                 readCompleted(error, 0);
                             }
                         });
}

Message TcpClient::get()
{
    std::unique_lock lock(mtx);
    if(messageQueue.empty())
    {
        return Message("");
    }
    Message m = messageQueue.front();
    messageQueue.pop();
    return m;
}

void TcpClient::readCompleted(const boost::system::error_code &error, std::size_t n)
{
    if (!error)
    {
        std::unique_lock lock(mtx);
        if (n > 0)
        {
            Message m({buffer.cbegin(), buffer.cbegin() + n});
            messageQueue.push(m);
        }
        socket.async_read_some(boost::asio::buffer(buffer.data(), buffer.size()),
                               std::bind(&TcpClient::readCompleted, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
    }
}



} // namespace CoffeeShop
