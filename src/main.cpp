#include "v8pp-lab/runner.hpp"
#include "v8pp-lab/scripts.hpp"
#include "v8pp-lab/filelocator.hpp"
#include "v8pp-lab/context.hpp"
#include "engine/coffeeshopmodule.hpp"
#include "visualisation/visualiser.hpp"
#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"
#include "messagequeue/producer.hpp"

#include <thread>

void runV8(std::unique_ptr<CoffeeShop::Producer> producer)
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
}

void runVisualizer(std::unique_ptr<CoffeeShop::Consumer> consumer)
{
    CoffeeShop::Visualiser(std::move(consumer)).run();
}

int main()
{
    auto bus = std::make_shared<CoffeeShop::MessageBus>();
    auto consumer = std::make_unique<CoffeeShop::Consumer>(bus);
    auto producer = std::make_unique<CoffeeShop::Producer>(bus);

    std::jthread v8Thread([p = std::move(producer)]() mutable {
        runV8(std::move(p));
    });

    std::jthread visThread([c = std::move(consumer)]() mutable {
        runVisualizer(std::move(c));
    });


    return 0;

}
