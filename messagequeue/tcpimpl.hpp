#pragma once
#include "messagequeue/message.hpp"
#include <queue>
#include <thread>
#include <functional>
#include <boost/asio.hpp>
#include "simplequeuedriver.hpp"
namespace CoffeeShop
{

class TcpClient
{
public:
    TcpClient();
    ~TcpClient();
    std::optional<Message> tryPop();
    std::optional<Message> waitAndTryPop();
    void readCompleted(const boost::system::error_code &error, std::size_t n);
    bool alive() const;
private:
    boost::asio::io_context ioContext;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard;
    boost::asio::ip::tcp::socket socket;
    std::jthread thread;
    SimpleQueueDriver safeQueue;
    std::array<char, 1024> buffer;
    std::atomic<bool> connectionBroken {false};
};

class TcpConnection
{
public:
    TcpConnection(boost::asio::io_context* context, std::stop_token stopToken);

    void push(const Message& message);
    boost::asio::ip::tcp::socket& getSocket();

    std::function<void(TcpConnection*)> onDisconnect;
private:
    void writeCompleted(const boost::system::error_code& error);
    boost::asio::io_context* ioContext;
    boost::asio::ip::tcp::socket socket;
    boost::asio::io_context::strand connectionStrand;
    bool writeInProgress {false};
    std::stop_token stopToken;

    std::queue<Message> messageQueue;
    std::mutex mtx {};
    std::array<char, 1024> buffer;
};


class TcpServer
{
public:
    TcpServer();
    ~TcpServer();
    void push(const Message& message);
    bool alive() const;
private:
    void listen();
    std::stop_source stopSource {};
    boost::asio::thread_pool pool;
    boost::asio::io_context ioContext;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::io_context::strand serverStrand;
    std::vector<std::shared_ptr<TcpConnection>> activeConnections {};
};

} // namespace CoffeeShop
