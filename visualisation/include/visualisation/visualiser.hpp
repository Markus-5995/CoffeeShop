#pragma once
#include "messagequeue/consumer.hpp"
#include "iostream"
#include <boost/archive/text_iarchive.hpp>
#include <sstream>
#include "concepts/world.hpp"
#include "SFML/Graphics.hpp"
#include <format>
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
