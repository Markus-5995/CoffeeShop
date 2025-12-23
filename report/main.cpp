#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"
#include "report/simulationreport.hpp"
#include <filesystem>

int main(int argc, char *argv[])
{
    using namespace CoffeeShop;
    auto bus = std::make_shared<MessageBus>(MessageBus::TcpIp);
    SimulationReport report(std::make_unique<Consumer>(bus));

    std::filesystem::path xmlFile = std::filesystem::current_path() / "MyXML.xml";
    report.run(xmlFile.string());
    return 0;
}
