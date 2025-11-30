#include "concepts/simulationcontext.hpp"

namespace CoffeeShop
{

bool SimulationContext::transferOwnership(Owner *newOwner, Actor *target)
{
    if (newOwner == nullptr || target == nullptr)
    {
        return false;
    }

    if (! world.m_actors.contains(target))
    {
        return false;
    }

    if (world.m_actors[target] != nullptr)
    {
        return false;
    }

    world.m_actors[target] = newOwner;
    return true;
}

std::vector<Actor *> SimulationContext::actors()
{
    std::vector<Actor*> keys;
    keys.reserve(world.m_actors.size());
    for(const auto& actor : world.m_actors)
    {
        keys.push_back(actor.first);
    }
    return keys;
}

SimulationContext::SimulationContext(const std::vector<Actor *> &newActors)
{
    for(const auto& actor : newActors)
    {
        world.m_actors[actor] = nullptr;
    }
}

void SimulationContext::runSimulation()
{
    for (const auto& actor : world.m_actors)
    {
        actor.first->act();
    }
    for (const auto& actor : world.m_actors)
    {
        actor.first->interact(*this);
    }
    world.m_runtime++;
}

int SimulationContext::runtime() const
{
    return world.m_runtime;
}

}
