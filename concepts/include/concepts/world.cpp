#include "concepts/world.hpp"
#include <ranges>
namespace CoffeeShop
{

bool World::transferOwnership(Owner *newOwner, Actor *target)
{
    if (target == nullptr)
    {
        return false;
    }

    auto it = std::find_if(m_actors.begin(), m_actors.end(),
                           [target](const auto& entry)
                           {
                               return entry.first.get() == target;
                           });

    if (it == m_actors.end())
    {
        return false;
    }

    if ((*it).second != nullptr && newOwner != nullptr)
    {
        return false;
    }

    (*it).second = newOwner;
    return true;
}

std::vector<Actor *> World::actors(uint32_t types)
{
    auto view = m_actors
                | std::views::filter([types](const auto& actorPair){return actorPair.first->type() & types;})
                | std::views::transform([](const auto& actorPair){return actorPair.first.get();});

    return {view.begin(), view.end()};
}

std::vector<Actor *> World::getOwnedActors(Owner *owner, uint32_t types)
{
    auto view = m_actors
                | std::views::filter([owner](const auto& actorPair){return actorPair.second == owner;})
                | std::views::filter([types](const auto& actorPair){return actorPair.first->type() & types;})
                | std::views::transform([](const auto& actorPair){return actorPair.first.get();});

    return {view.begin(), view.end()};
}


}

