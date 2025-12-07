#pragma once
#include "world.hpp"

namespace CoffeeShop
{

struct SimulationContext
{
    SimulationContext(const std::vector<std::shared_ptr<Actor>>& newActors);
    void runSimulation();
    World world {};
    static inline uint32_t nextId = 1;
};

}
