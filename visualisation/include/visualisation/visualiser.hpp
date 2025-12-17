#pragma once
#include "messagequeue/consumer.hpp"
#include "visualisationexport.hpp"
namespace CoffeeShop
{

class VIS_API Visualiser
{
public:
    Visualiser(std::unique_ptr<Consumer> consumer);
    void run();

private:
    std::unique_ptr<Consumer> consumer {};
};

} // namespace CoffeeShop
