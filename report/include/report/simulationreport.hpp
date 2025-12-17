#pragma once
#include "messagequeue/consumer.hpp"
#include <stop_token>
#include "reportexport.hpp"
namespace CoffeeShop
{

class REPORT_API SimulationReport
{
public:
    SimulationReport(std::unique_ptr<Consumer> consumer);
    void run(std::string_view path, std::stop_token stop = std::stop_token());

private:
    std::unique_ptr<Consumer> consumer;
};

} // namespace CoffeeShop
