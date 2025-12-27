#include "jsonsim.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#include <thread>
#include <chrono>
namespace CoffeeShop
{
	constexpr std::string_view jsonTimeout = "stop";
	constexpr std::string_view jsonInventory = "inventory";
	constexpr std::string_view jsonType = "type";
	constexpr std::string_view jsonMug = "mug";
	constexpr std::string_view jsonCoffeeMachine = "coffeemachine";

	JsonSimulation::JsonSimulation(std::unique_ptr<Producer> producer, std::string path) :
		m_producer(std::move(producer))
	{
		using json = nlohmann::json;
		std::ifstream stream(path);
		json root = json::parse(stream);
		m_timeout = root[jsonTimeout];
		json inventory = root[jsonInventory];
		std::vector<std::shared_ptr<Actor>> actors;
		for (const json item : inventory)
		{
			std::string type = item[jsonType];
			if (type == jsonMug)
			{
				actors.push_back(std::make_shared<Mug>());
			}
			else if (type == jsonCoffeeMachine)
			{
				actors.push_back(std::make_shared<CoffeeMachine>());
			}
		}

		m_context = std::make_unique<SimulationContext>(actors);
	}

	void JsonSimulation::run()
	{
		while (m_context->getWorld().m_runtime <= m_timeout)
		{
			m_context->runSimulation();
			std::stringstream ss{};
			boost::archive::binary_oarchive archive{ ss };
			archive << m_context->getWorld();

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1s);
			m_producer->push(Message(ss.str()));
		}
	}

}