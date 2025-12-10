#pragma once
#include "messagequeue/producer.hpp"
#include <stop_token>
namespace CoffeeShop
{

class Engine
{
public:
    explicit Engine(std::unique_ptr<Producer> producer);
    void run(std::stop_source&& stopSrc);
private:
    std::unique_ptr<Producer> producer;
};

} // namespace CoffeeShop
