#include "tcpclient.hpp"

namespace CoffeeShop
{
	using namespace boost::asio;
	using namespace boost::asio::ip;
	using error_code = boost::system::error_code;

	constexpr port_type port = 5000;
	constexpr std::string_view hostAddress = "127.0.0.1";

	TcpClient::TcpClient():
		m_ioContext(),
		m_guard(boost::asio::make_work_guard(m_ioContext)),
		m_socket(m_ioContext),
		m_thread([this]() {m_ioContext.run(); })

	{
		auto addr = make_address(hostAddress);
		m_socket.async_connect(tcp::endpoint(addr, port),
			[this](const error_code& error)
			{
				if (m_thread.get_stop_token().stop_requested())
				{
					return;
				}
				if (!error)
				{
					readCompleted(error);
				}
			});
	}

	TcpClient::~TcpClient()
	{
		m_thread.get_stop_source().request_stop();
		m_safeQueue.close();
		m_socket.close();
		m_guard.reset();
		m_ioContext.stop();
	}

	std::optional<Message> TcpClient::tryPop()
	{
		return m_safeQueue.tryPop(0);
	}

	std::optional<Message> TcpClient::waitAndTryPop()
	{
		return m_safeQueue.waitAndTryPop(0);
	}

	void TcpClient::readCompleted(const error_code& error,  Message m)
	{
		if (m_thread.get_stop_token().stop_requested())
		{
			return;
		}
		if (!error)
		{
			if (m.size() > 0)
			{
				m_safeQueue.push(m);
			}
			async_readMessage(m_socket,
				std::bind(&TcpClient::readCompleted, this,
					placeholders::error,
					placeholders::results));
		}
		else
		{
			m_connectionBroken.store(true);
			m_safeQueue.close();
		}
	}

	bool TcpClient::alive() const
	{
		return !m_connectionBroken.load();
	}


}