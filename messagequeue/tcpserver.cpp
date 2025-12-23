#include "tcpserver.hpp"
#include <iostream>
namespace CoffeeShop
{
	using namespace boost::asio;
	using namespace boost::asio::ip;
	using error_code = boost::system::error_code;

	constexpr port_type port = 5000;
	constexpr std::string_view hostAddress = "127.0.0.1";

	TcpConnection::TcpConnection(io_context* context, std::stop_token stopToken) :
		ioContext(context),
		socket(*ioContext),
		connectionStrand(*ioContext),
		stopToken(stopToken)
	{

	}

	void TcpConnection::push(const Message& message)
	{
		{
			// This is not posted as an io context to make sure the messages are in order
			std::lock_guard lock(mtx);
			messageQueue.push(message);
			if (writeInProgress)
				return;
			writeInProgress = true;
		}

		if (!stopToken.stop_requested())
		{
			post(*ioContext,
				bind_executor(connectionStrand, [this] {writeCompleted(error_code()); })
			);
		}
	}

	tcp::socket& TcpConnection::getSocket()
	{
		return socket;
	}

	void TcpConnection::writeCompleted(const error_code& error)
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
		Message m = messageQueue.front();
		messageQueue.pop();
		sendMessage(m);
	}

	void TcpConnection::sendMessage(const Message& message)
	{
		header[0] = message.size();

		std::vector<boost::asio::const_buffer> buffers{
		boost::asio::buffer(header.data(), sizeof(std::uint32_t)),
		boost::asio::buffer(message.payload().data(), message.size())
		};
		std::cout << "Sending message(" << header.at(0) << ")..." << std::endl;
		boost::asio::async_write(socket, buffers,
			bind_executor(connectionStrand,
				std::bind(&TcpConnection::writeCompleted,
					this, placeholders::error)));
	}



	TcpServer::TcpServer() :
		ioContext(),
		guard(boost::asio::make_work_guard(ioContext)),
		acceptor(ioContext, tcp::endpoint(tcp::v4(), port)),
		serverStrand(ioContext)
	{
		for (int i = 0; i < 4; i++)
		{
			workers.emplace_back(
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
	}

	void TcpServer::push(const Message& message)
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

	bool TcpServer::alive() const
	{
		return true;
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
								[c](const auto& ptr) {return ptr.get() == c; }),
							activeConnections.end());
					});
			};

		acceptor.async_accept(connection->getSocket(),
			bind_executor(serverStrand,
				[this, connection](const error_code& error)
				{
					if (!error)
					{
						activeConnections.push_back(connection);
					}
					if (!stopSource.stop_requested())
					{
						listen();
					}
				}));

	}

} // namespace CoffeeShop
