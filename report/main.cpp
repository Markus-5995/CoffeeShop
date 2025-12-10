#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"
#include "report/simulationreport.hpp"

int main(int argc, char *argv[])
{
    using namespace CoffeeShop;
    auto bus = std::make_shared<MessageBus>(MessageBus::TcpIp);
    SimulationReport report(std::make_unique<Consumer>(bus));

    report.run();
    return 0;
}
