#include "concepts/coffeemachine.hpp"
#include "concepts/simulationcontext.hpp"
namespace CoffeeShop
{

void CoffeeMachine::act()
{
    // Heat up machine to be ready...
    if (temperature <= 40)
    {
        temperature+= 5;
    }
}

void CoffeeMachine::interact(SimulationContext &sim)
{
    // If machine is hot, it should take a mug
    if (temperature >= 40)
    {
        // Search for a mug nearby...
        if (mug == nullptr)
        {
            auto actors = sim.actors();
            for (auto* actor : actors)
            {
                if (auto* potentialMug = dynamic_cast<Mug*>(actor))
                {
                    if (sim.transferOwnership(this, potentialMug))
                    {
                        if (! potentialMug->full())
                        {
                            mug = potentialMug;
                            return;
                        }
                    }
                }
            }
        }
        // Already found mug
        else
        {
            if (! mug->full())
            {
                mug->fillStand += 10;
                // Cant make it hotter than the coffee itself
                mug->temperature = std::min(mug->temperature + 10, temperature);
            }
            else
            {
                // Be free mug!!
                sim.transferOwnership(nullptr, mug);
            }
        }
    }
}

}
