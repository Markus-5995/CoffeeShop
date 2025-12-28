#include "include/engine/engine.hpp"
#include "JSONEngine/jsonsim.hpp"
#include "CppEngine/simulation.hpp"
#include "V8Engine/simulation.hpp"


namespace CoffeeShop
{
	Engine::Engine(std::unique_ptr<Producer> producer) :
		producer(std::move(producer))
	{

	}

	void Engine::run(std::stop_source&& stopSrc, EngineType engineType)
	{
		switch (engineType)
		{
		case EngineType::V8Engine:
			V8Simulation(std::move(producer)).run();
			break;
		case EngineType::CppEngine:
			CppSimulation(std::move(producer)).run();
			break;
		case EngineType::JSONEngine:
			JsonSimulation(std::move(producer)).run();
			break;
		}
		stopSrc.request_stop();
	}

} // namespace CoffeeShop
