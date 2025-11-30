#include "concepts/mug.hpp"

namespace CoffeeShop
{

void Mug::setTemperatureThreshold(int threshold)
{
    s_threshold = threshold;
}

void Mug::act()
{
    // A mug is just cooling of in a linear fashion
    temperature -= 1;
}

bool Mug::full() const
{
    return fillStand >= 100;
}

bool Mug::hot() const
{
    return temperature >= s_threshold;
}

}
