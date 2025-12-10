#include "include/engine/engine.hpp"
#include "v8pp-lab/runner.hpp"
#include "v8pp-lab/scripts.hpp"
#include "v8pp-lab/filelocator.hpp"
#include "v8pp-lab/context.hpp"
#include "coffeeshopmodule.hpp"

namespace CoffeeShop
{

Engine::Engine(std::unique_ptr<Producer> producer) :
    producer(std::move(producer))
{

}

void Engine::run(std::stop_source &&stopSrc)
{
    using namespace V8ppLab;
    FileLocator locator (DEFAULT_JS_DIR);
    std::tuple<std::vector<FileScript>> scripts{ locator.getScripts() };

    CoffeeShop::CppModule coffeeModule(std::move(producer));
    std::vector<CoffeeShop::CppModule> vec;
    vec.push_back(std::move(coffeeModule));
    std::tuple<std::vector<CoffeeShop::CppModule>> modules{ std::move(vec) };


    Runner<V8Context> runner {};
    runner.loadModules(modules);
    runner.run(scripts);
    stopSrc.request_stop();
}

} // namespace CoffeeShop
