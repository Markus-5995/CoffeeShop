#pragma once
#include "messagequeue/consumer.hpp"
#include <stop_token>
namespace CoffeeShop
{

class SimulationReport
{
public:
    SimulationReport(std::unique_ptr<Consumer> consumer);
    void run(std::stop_token stop);

private:
    std::unique_ptr<Consumer> consumer;
};

} // namespace CoffeeShop
