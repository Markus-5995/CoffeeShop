#include "messagequeue/producer.hpp"
#include "messagequeue/messagebus.hpp"
#include "engine/engine.hpp"
#include "iostream"

int main(int argc, char *argv[])
{
    using namespace CoffeeShop;
    auto bus = std::make_shared<MessageBus>(MessageBus::TcpIp);
    Engine engine(std::make_unique<Producer>(bus));

    engine.run(std::stop_source());
    return 0;
}
