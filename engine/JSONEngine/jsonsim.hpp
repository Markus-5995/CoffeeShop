#include "messagequeue/producer.hpp"
#include "concepts/simulationcontext.hpp"

namespace CoffeeShop
{
	class JsonSimulation
	{

	public:
		JsonSimulation(std::unique_ptr<Producer> producer, std::string path);
		void run();

	private:
		int m_timeout;
		std::unique_ptr<Producer> m_producer;
		std::unique_ptr<SimulationContext> m_context;
	};
}