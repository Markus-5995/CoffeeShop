#include "visualisation/visualiser.hpp"
#include "messagequeue/consumer.hpp"
#include "messagequeue/messagebus.hpp"


int main(int argc, char *argv[])
{
    using namespace CoffeeShop;
    auto bus = std::make_shared<MessageBus>(MessageBus::TcpIp);
    Visualiser vis(std::make_unique<Consumer>(bus));

    vis.run();
    return 0;
}
