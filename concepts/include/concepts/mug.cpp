#include "concepts/mug.hpp"

namespace CoffeeShop
{


void Mug::act()
{
    // A mug is just cooling of in a linear fashion
    temperature = std::max(0, temperature - 5);
}

uint32_t Mug::type() const
{
    return ActorTypes::MugType;
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
