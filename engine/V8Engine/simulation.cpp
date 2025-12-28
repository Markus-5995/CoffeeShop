#include "simulation.hpp"
#include <stdexcept>
#ifdef V8_SUPPORT
#include "v8pp-lab/runner.hpp"
#include "v8pp-lab/scripts.hpp"
#include "v8pp-lab/filelocator.hpp"
#include "v8pp-lab/context.hpp"
#include "coffeeshopmodule.hpp"
#endif // V8_SUPPORT

namespace CoffeeShop
{
	V8Simulation::V8Simulation(std::unique_ptr<Producer> producer, std::string path) :
		m_producer(std::move(producer)),
		m_path(std::move(path))
	{

	}
	void V8Simulation::run()
	{
#ifdef V8_SUPPORT
		using namespace V8ppLab;
		FileLocator locator(m_path);
		std::tuple<std::vector<FileScript>> scripts{ locator.getScripts() };

		CoffeeShop::CppModule coffeeModule(std::move(m_producer));
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
}