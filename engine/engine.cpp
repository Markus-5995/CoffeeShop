#include "include/engine/engine.hpp"
#include <stdexcept>
#ifdef V8_SUPPORT
#include "v8pp-lab/runner.hpp"
#include "v8pp-lab/scripts.hpp"
#include "v8pp-lab/filelocator.hpp"
#include "v8pp-lab/context.hpp"
#include "coffeeshopmodule.hpp"
#endif // V8_SUPPORT
#include "JSONEngine/jsonsim.hpp"
#include "CppEngine/simulation.hpp"


namespace CoffeeShop
{
	void runV8(std::unique_ptr<Producer> producer)
	{
#ifdef V8_SUPPORT
		using namespace V8ppLab;
		FileLocator locator(DEFAULT_JS_DIR);
		std::tuple<std::vector<FileScript>> scripts{ locator.getScripts() };

		CoffeeShop::CppModule coffeeModule(std::move(producer));
		std::vector<CoffeeShop::CppModule> vec;
		vec.push_back(std::move(coffeeModule));
		std::tuple<std::vector<CoffeeShop::CppModule>> modules{ std::move(vec) };


		Runner<V8Context> runner{};
		runner.loadModules(modules);
		runner.run(scripts);
		stopSrc.request_stop();
#else
		throw std::runtime_error("V8 not supported!");
#endif // V8_SUPPORT
	}


	void runCpp(std::unique_ptr<Producer> producer)
	{
		CppSimulation simulation(std::move(producer));
		simulation.run();
	}

	void runJSON(std::unique_ptr<Producer> producer)
	{
		JsonSimulation simulation(std::move(producer), DEFAULT_JSON_DIR);
		simulation.run();
	}

	Engine::Engine(std::unique_ptr<Producer> producer) :
		producer(std::move(producer))
	{

	}

	void Engine::run(std::stop_source&& stopSrc, EngineType engineType)
	{
		switch (engineType)
		{
		case EngineType::V8Engine:
			runV8(std::move(producer));
			break;
		case EngineType::CppEngine:
			runCpp(std::move(producer));
			break;
		case EngineType::JSONEngine:
			runJSON(std::move(producer));
			break;
		}
		stopSrc.request_stop();
	}

} // namespace CoffeeShop
