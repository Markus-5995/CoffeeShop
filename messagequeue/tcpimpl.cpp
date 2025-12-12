#include "tcpimpl.hpp"
#include <iostream>
namespace CoffeeShop
{

using namespace boost::asio;
using namespace boost::asio::ip;
using error_code = boost::system::error_code;

constexpr port_type port = 5000;
constexpr std::string_view hostAddress = "127.0.0.1";

TcpClient::TcpClient() :
    ioContext(),
    guard(boost::asio::make_work_guard(ioContext)),
    socket(ioContext),
    thread([this]() {ioContext.run();})

{
    auto addr = make_address(hostAddress);
    socket.async_connect(tcp::endpoint(addr, port),
                         [this](const error_code &error)
                         {
                             if (thread.get_stop_token().stop_requested())
                             {
                                 return;
                             }
                             if (! error)
                             {
                                 readCompleted(error, 0);
                             }
                         });
}

TcpClient::~TcpClient()
{
    thread.get_stop_source().request_stop();
    socket.close();
    guard.reset();
    ioContext.stop();
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

void TcpClient::readCompleted(const error_code &error, std::size_t n)
{
    if (thread.get_stop_token().stop_requested())
    {
        return;
    }
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
                                         placeholders::error,
                                         placeholders::bytes_transferred));
    }
}


TcpConnection::TcpConnection(io_context *context, std::stop_token stopToken) :
    ioContext(context),
    socket(*ioContext),
    connectionStrand(*ioContext),
    stopToken(stopToken)
{

}

void TcpConnection::push(const Message &message)
{
    {
        // This is not posted as an io context to make sure the messages are in order
        std::lock_guard lock(mtx);
        messageQueue.push(message);
        if (writeInProgress)
            return;
        writeInProgress = true;
    }

    if (! stopToken.stop_requested())
    {
        post(*ioContext,
             bind_executor(connectionStrand, [this]{writeCompleted(error_code());})
             );
    }
}

tcp::socket &TcpConnection::getSocket()
{
    return socket;
}

void TcpConnection::writeCompleted(const error_code &error)
{
    if (stopToken.stop_requested())
    {
        return;
    }

    if (error)
    {
        socket.close();
        if (onDisconnect)
        {
            onDisconnect(this);
        }
        return;
    }

    std::unique_lock lock(mtx);
    if (messageQueue.empty())
    {
        writeInProgress = false;
        return;
    }
    writeInProgress = true;
    std::string m = messageQueue.front();
    messageQueue.pop();
    if (m.size() > buffer.size())
    {
        throw std::runtime_error("We are in trouble!");
    }
    std::fill(buffer.begin(), buffer.end(), 0);
    std::copy(m.begin(), m.end(), buffer.begin());

    socket.async_write_some(boost::asio::buffer(buffer.data(), buffer.size()),
                            bind_executor(connectionStrand,
                                          std::bind(&TcpConnection::writeCompleted,
                                                    this, placeholders::error)));
}


TcpServer::TcpServer() :
    pool(4),
    ioContext(),
    guard(boost::asio::make_work_guard(ioContext)),
    acceptor(ioContext, tcp::endpoint(tcp::v4(), port)),
    serverStrand(ioContext)
{
    for (int i = 0; i < 4; i++)
    {
        boost::asio::post(pool,
                          [this]()
                          {
                              ioContext.run();
                              std::cout << "Stopping Server Thread" << std::endl;
                          });
    }

    listen();
}

TcpServer::~TcpServer()
{
    stopSource.request_stop();
    guard.reset();
    ioContext.stop();
    pool.stop();
}

void TcpServer::push(const Message &message)
{
    post(ioContext, bind_executor(serverStrand,
                                  [this, message]
                                  {
                                      for (auto& ptr : activeConnections)
                                      {
                                          ptr->push(message);
                                      }
                                  }));
}

void TcpServer::listen()
{
    if (stopSource.stop_requested())
    {
        return;
    }

    auto connection = std::make_shared<TcpConnection>(&ioContext, stopSource.get_token());
    connection->onDisconnect = [this](TcpConnection* c)
    {
        post(serverStrand,
             [this, c]
             {
                 std::cout << "Removing broken connection!" << std::endl;
                 activeConnections.erase(
                     std::remove_if(activeConnections.begin(), activeConnections.end(),
                                    [c](const auto& ptr) {return ptr.get() == c;}),
                     activeConnections.end());
             });
    };

    acceptor.async_accept(connection->getSocket(),
                          bind_executor(serverStrand,
                                        [this, connection] (const error_code &error)
                                        {
                                            if (! error)
                                            {
                                                activeConnections.push_back(connection);
                                            }
                                            if (! stopSource.stop_requested())
                                            {
                                                listen();
                                            }
                                        }));

}

} // namespace CoffeeShop
