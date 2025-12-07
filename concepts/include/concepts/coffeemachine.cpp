#include "concepts/coffeemachine.hpp"
#include "concepts/simulationcontext.hpp"
#include <iostream>
namespace CoffeeShop
{

void CoffeeMachine::act()
{
    // Heat up machine to be ready...
    if (temperature <= temperatureLimit)
    {
        temperature =  std::min(temperatureLimit, temperature + 5);
    }
}

void CoffeeMachine::interact(World &world)
{
    // Heat up the machine first
    if (temperature < temperatureLimit)
    {
        return;
    }

    // Check if new mugs can be placed...
    auto ownedActors = world.getOwnedActors(this, ActorTypes::MugType);

    // Search for a mug nearby...
    if (ownedActors.size() < slots)
    {
        auto actors = world.actors(ActorTypes::MugType);
        for (auto* actor : actors)
        {
            if (auto* potentialMug = dynamic_cast<Mug*>(actor))
            {
                if (! potentialMug->full())
                {
                    if (world.transferOwnership(this, potentialMug))
                    {
                        std::cout << "Snatching a mug" << potentialMug->id() << std::endl;
                        return;
                    }
                }
            }
        }
    }

    // Already found mug
    for (auto* ownedMug : ownedActors)
    {
        if (auto* mug = dynamic_cast<Mug*>(ownedMug))
        {
            if (! mug->full())
            {
                mug->fillStand += 10;
                // Cant make it hotter than the machine itself
                mug->temperature = std::min(mug->temperature + 10, temperature);
            }
            else
            {
                // Be free mug!!
                world.transferOwnership(nullptr, mug);

                std::cout << "Dropping a mug" << mug->id() << std::endl;
            }
        }
    }
}

}
