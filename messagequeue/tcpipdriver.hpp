#pragma once
#include "messagequeue/message.hpp"
#include <queue>
#include <thread>
#include <boost/asio.hpp>
namespace CoffeeShop
{

class Resources
{
public:
    Resources(std::shared_ptr<boost::asio::io_context> context);
protected:
    std::shared_ptr<boost::asio::io_context> ioContext;
    std::mutex mtx {};
    std::queue<Message> messageQueue;
    boost::asio::ip::tcp::socket socket;
    std::array<char, 1024> buffer;
};


class TcpServer : public Resources
{
public:
    TcpServer(std::shared_ptr<boost::asio::io_context> context);
    void push(const Message& message);
private:
    void writeCompleted(const boost::system::error_code& error);
    boost::asio::ip::tcp::acceptor acceptor;
};

class TcpClient : public Resources
{
public:
    TcpClient(std::shared_ptr<boost::asio::io_context> context);
    Message get();

    void readCompleted(const boost::system::error_code &error, std::size_t);
};

class TcpIpDriver
{
public:
    TcpIpDriver();
    ~TcpIpDriver();
    void registerAsServer();
    void registerAsClient();

    void push(const Message& message);
    Message get();
    static constexpr inline boost::asio::ip::port_type port = 5000;
    static constexpr inline std::string_view hostAddress = "127.0.0.1";
private:
    std::shared_ptr<boost::asio::io_context> io;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> workGuard;
    std::jthread thread;
    std::variant<std::monostate, TcpServer, TcpClient> communciation;
};

} // namespace CoffeeShop
