#include "simulation.hpp"
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include <thread>
#include <chrono>

namespace CoffeeShop
{
	CppSimulation::CppSimulation(std::unique_ptr<Producer> producer) :
		m_producer(std::move(producer))
	{

	}

	void CppSimulation::run()
	{
		int timeout = 25;
		std::vector<std::shared_ptr<Actor>> actors;

		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<Mug>());
		actors.push_back(std::make_shared<CoffeeMachine>());
		actors.push_back(std::make_shared<CoffeeMachine>());

		context = std::make_unique<SimulationContext>(std::move(actors));
		while (context->getWorld().m_runtime < timeout)
		{
			context->runSimulation();

			std::stringstream ss{};
			boost::archive::binary_oarchive archive{ ss };
			archive << context->getWorld();

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1s);
			m_producer->push(Message(ss.str()));
		}
	}
}