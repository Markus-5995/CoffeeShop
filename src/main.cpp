#include "v8pp-lab/runner.hpp"
#include "v8pp-lab/scripts.hpp"
#include "v8pp-lab/filelocator.hpp"
#include "v8pp-lab/context.hpp"
#include "engine/coffeeshopmodule.hpp"
#include "visualisation/visualiser.hpp"
#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"
#include "messagequeue/producer.hpp"
#include "report/simulationreport.hpp"
#include <thread>

void runV8(std::unique_ptr<CoffeeShop::Producer> producer, std::stop_source&& src)
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
    src.request_stop();
}

void runVisualizer(std::unique_ptr<CoffeeShop::Consumer> consumer)
{
    CoffeeShop::Visualiser(std::move(consumer)).run();
}

void runReport(std::unique_ptr<CoffeeShop::Consumer> consumer, std::stop_token token)
{
    CoffeeShop::SimulationReport(std::move(consumer)).run(token);
}

int main()
{
    auto bus = std::make_shared<CoffeeShop::MessageBus>(CoffeeShop::MessageBus::MultiConsumerQueue);
    auto visconsumer = std::make_unique<CoffeeShop::Consumer>(bus);
    auto repconsumer = std::make_unique<CoffeeShop::Consumer>(bus);
    auto producer = std::make_unique<CoffeeShop::Producer>(bus);

    std::stop_source stopSrc;
    std::stop_token stopToken = stopSrc.get_token();

    std::jthread v8Thread([p = std::move(producer), s = std::move(stopSrc)]() mutable {
        runV8(std::move(p), std::move(s));
    });

    std::jthread visThread([c = std::move(visconsumer)]() mutable {
        runVisualizer(std::move(c));
    });

    std::jthread repThread([c = std::move(repconsumer), s = stopToken]() mutable {
        runReport(std::move(c), s);
    });

    return 0;

}
