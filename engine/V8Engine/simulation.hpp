#pragma once
#include <memory>
#include "messagequeue/producer.hpp"
namespace CoffeeShop
{
	class V8Simulation
	{
	public:
		V8Simulation(std::unique_ptr<Producer> producer, std::string path = DEFAULT_JS_DIR);
		void run();

	private:
		std::unique_ptr<Producer> m_producer;
		std::string m_path;
	};
}