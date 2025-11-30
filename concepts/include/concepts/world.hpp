#pragma once
#include "coffeemachine.hpp"
#include "mug.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/unordered_map.hpp"
#include <set>
namespace CoffeeShop
{
using Owner = Actor;

struct World
{
    World(bool owningMemory = false) :
        m_owning(owningMemory)
    {

    }

    ~World()
    {
        if (m_owning)
        {
            std::set<Actor*> lookup;
            for(auto& actor : m_actors)
            {
                if (actor.first != nullptr)
                {
                    auto [_, inserted] = lookup.insert(actor.first);
                    if (inserted)
                    {
                        delete actor.first;
                    }
                }
                if (actor.second != nullptr)
                {
                    auto [_, inserted] = lookup.insert(actor.second);
                    if (inserted)
                    {
                        delete actor.second;
                    }
                }
            }
        }
    }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar.register_type(static_cast<Mug *>(NULL));
        ar.register_type(static_cast<CoffeeMachine *>(NULL));
        ar & m_actors & m_runtime;
    }

    // Key are all m_actors in the simluation...
    // Values are who owns another actor
    std::unordered_map<Actor*, Owner*> m_actors;

    int m_runtime {};
    bool m_owning {};
};
}
