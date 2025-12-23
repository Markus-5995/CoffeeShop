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
		void readCompleted(const boost::system::error_code& error, Message m = Message());
		bool alive() const;
	private:
		boost::asio::io_context m_ioContext;
		boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_guard;
		boost::asio::ip::tcp::socket m_socket;
		std::jthread m_thread;
		SimpleQueueDriver m_safeQueue;
		std::atomic<bool> m_connectionBroken{ false };
	};

	struct ReadMessageImplementation
	{
		static constexpr size_t MAX_MESSAGE_SIZE = 1024 * 1024; // 1 MB
		boost::asio::ip::tcp::socket& m_socket;
		struct Context
		{
			enum Status
			{ 
				starting, 
				readingHeader, 
				readingMessage
			} m_state = starting;
			std::array<std::uint32_t, 1> m_header{};
			Message::PayLoad m_payload{};
		};
		std::unique_ptr<Context> ctx = std::make_unique<Context>();

		template <typename Self>
		void operator()(Self& self,
			boost::system::error_code error = {},
			Message message = Message())
		{
			switch (ctx->m_state)
			{
			case Context::starting:
			{
				ctx->m_state = Context::readingHeader;
				auto buf = boost::asio::buffer(ctx->m_header.data(), sizeof(std::uint32_t));
				boost::asio::async_read(m_socket, buf,
					[self = std::move(self)] (boost::system::error_code error, size_t n) mutable
					{
						self(error);
					});
				break;
			}
			case Context::readingHeader:
			{
				if (error)
				{
					ctx->m_state = Context::starting;
					self.complete(error, Message());
				}
				else
				{
					ctx->m_state = Context::readingMessage;
					size_t messageSize = ctx->m_header.at(0);
					if (messageSize <= 0 || messageSize > MAX_MESSAGE_SIZE)
					{
						self.complete(boost::system::error_code(), Message());
						break;
					}
					ctx->m_payload.resize(messageSize);
					auto buf = boost::asio::buffer(ctx->m_payload.data(), ctx->m_payload.size());
					boost::asio::async_read(m_socket, buf,
						[self = std::move(self)] (boost::system::error_code error, size_t n) mutable
						{
							self(error);
						});
				}
				break;
			}
			case Context::readingMessage:
			{
				ctx->m_state = Context::starting;
				self.complete(error, Message(std::move(ctx->m_payload)));
				break;
			}
			}
		}
	};

	template <typename CompletionToken>
	auto async_readMessage(boost::asio::ip::tcp::socket& socket,
		CompletionToken&& token)
		-> decltype(
			boost::asio::async_compose<CompletionToken,
			void(boost::system::error_code, Message)>(
				std::declval<ReadMessageImplementation>(),
				token, socket))
	{
		return boost::asio::async_compose<CompletionToken,
			void(boost::system::error_code, Message)>(
				ReadMessageImplementation{ socket },
				token, socket);
	}

}