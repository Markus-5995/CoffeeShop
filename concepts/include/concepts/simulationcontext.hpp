#pragma once
#include "world.hpp"

namespace CoffeeShop
{

struct SimulationContext
{
    bool transferOwnership(Owner* newOwner, Actor* target);
    std::vector<Actor*> actors();

    SimulationContext(const std::vector<Actor*>& newActors);

    void runSimulation();

    int runtime() const;
    World world {};
};

}
