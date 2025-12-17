#include "engine/engine.hpp"
#include "visualisation/visualiser.hpp"
#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"
#include "messagequeue/producer.hpp"
#include "report/simulationreport.hpp"
#include <thread>
#include <filesystem>

void runEngine(std::unique_ptr<CoffeeShop::Producer> producer, std::stop_source&& src)
{
    CoffeeShop::Engine(std::move(producer)).run(std::move(src));
}

void runVisualizer(std::unique_ptr<CoffeeShop::Consumer> consumer)
{
    CoffeeShop::Visualiser(std::move(consumer)).run();
}

void runReport(std::unique_ptr<CoffeeShop::Consumer> consumer, std::stop_token token)
{
    std::filesystem::path xmlFile = std::filesystem::current_path() / "MyXML.xml";
    CoffeeShop::SimulationReport(std::move(consumer)).run(xmlFile.string(), token);
}

int main()
{
    auto bus = std::make_shared<CoffeeShop::MessageBus>(CoffeeShop::MessageBus::MultiConsumerQueue);
    auto visconsumer = std::make_unique<CoffeeShop::Consumer>(bus);
    auto repconsumer = std::make_unique<CoffeeShop::Consumer>(bus);
    auto producer = std::make_unique<CoffeeShop::Producer>(bus);

    std::stop_source stopSrc;
    std::stop_token stopToken = stopSrc.get_token();

    std::jthread simThread([p = std::move(producer), s = std::move(stopSrc)]() mutable {
        runEngine(std::move(p), std::move(s));
    });

    std::jthread visThread([c = std::move(visconsumer)]() mutable {
        runVisualizer(std::move(c));
    });

    std::jthread repThread([c = std::move(repconsumer), s = stopToken]() mutable {
        runReport(std::move(c), s);
    });

    return 0;

}
