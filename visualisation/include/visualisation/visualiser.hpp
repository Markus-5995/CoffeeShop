#pragma once
#include "messagequeue/consumer.hpp"

namespace CoffeeShop
{

class Visualiser
{
public:
    Visualiser(std::unique_ptr<Consumer> consumer);

    // BLOCKS
    void run();

private:
    std::unique_ptr<Consumer> consumer {};
};

} // namespace CoffeeShop
