#pragma once
#include <string_view>
#include <memory>
#include "messagequeue/producer.hpp"
#include "concepts/simulationcontext.hpp"
namespace CoffeeShop
{
	class JsonSimulation
	{
	public:
		JsonSimulation(std::unique_ptr<Producer> producer, std::string_view path);
		void run();

	private:
		std::unique_ptr<Producer> m_producer;
		std::unique_ptr<SimulationContext> context;
	};
}
