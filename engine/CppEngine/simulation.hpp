#pragma once
#include <string_view>
#include <memory>
#include "messagequeue/producer.hpp"
#include "concepts/simulationcontext.hpp"
namespace CoffeeShop
{
	class CppSimulation
	{
	public:
		CppSimulation(std::unique_ptr<Producer> producer);
		void run();

	private:
		std::unique_ptr<Producer> m_producer;
		std::unique_ptr<SimulationContext> context;
	};
}
